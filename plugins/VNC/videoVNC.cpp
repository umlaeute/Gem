#include "videoVNC.h"
#include "plugins/PluginFactory.h"

#include <m_pd.h>

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("vnc", videoVNC);

static double getRandom(void)
{
  static unsigned int random_nextseed = 1489853723;
  random_nextseed = random_nextseed * 435898247 + 938284281;
  return random_nextseed * (1./4294967296.);;
}

static videoVNC*rfb2gem(rfbClient*client)
{
  return (videoVNC*)rfbClientGetClientData(client, (void*)(rfb2gem));
}



videoVNC::videoVNC(void)
  : m_name(std::string("vnc"))
  , m_client(0)
{
  m_mouse.x = -1;
  m_mouse.y = -1;
  m_mouse.mask=0;
  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setFormat(GEM_RGBA);
  m_pixBlock.image.reallocate();
}

videoVNC::~videoVNC(void)
{
  close();
}

void videoVNC::close(void)
{
  if(m_client) {
    rfbClientCleanup(m_client);
    m_client=0;
  }
}


bool videoVNC::open(gem::Properties&props)
{
  if(m_devname.empty()) {
    return false;
  }
  setProperties(props);
  if(m_client) {
    close();
  }
  m_client=rfbGetClient(8,3,4);
  if(!m_client) {
    return false;
  }

  m_client->GotFrameBufferUpdate = frameBufferCB;
  m_client->GetPassword          = passwordCB;
  rfbClientSetClientData(m_client, (void*)(rfb2gem), this);

  if(true) {
    char*devname=strdup(m_devname.c_str());
    char*progname=strdup("gem");
    char*argv[]= {
      progname, // fake program name
      devname // the connection string
    };
    int argc=sizeof(argv)/sizeof(*argv);
    rfbBool res=rfbInitClient(m_client, &argc, argv);
    free(devname);
    free(progname);
    if(!res) {
      // rfbInitClient() will call rfbClientCleanup() on failure automatically!
      m_client=0;
      return false;
    }
  }
  return true;
}

pixBlock*videoVNC::getFrame(void)
{
  if(m_client) {
    // process all RFB-messages so far...
    int w=WaitForMessage(m_client, 5);
    if(w>0) {
      if(!HandleRFBServerMessage(m_client)) {
        //return 0;
      }
    }
  }
  return &m_pixBlock;
}

std::vector<std::string>videoVNC::enumerate(void)
{
  std::vector<std::string>result;
  result.push_back("vnc");
  return result;
}

bool videoVNC::setDevice(int ID)
{
  m_devname.clear();
  return false;
}
bool videoVNC::setDevice(const std::string&device)
{
  m_devname.clear();
  const std::string prefix="vnc://";
  if (!device.compare(0, prefix.size(), prefix)) {
    m_devname=device.substr(prefix.size());
    return true;
  }
  return false;
}
bool videoVNC::enumProperties(gem::Properties&readable,
                              gem::Properties&writeable)
{
  std::string dummy_s;
  int dummy_i=0;
  readable.clear();
  writeable.clear();

  readable.set("width", m_pixBlock.image.xsize);
  readable.set("height", m_pixBlock.image.ysize);

  writeable.set("password", dummy_s);

  writeable.set("mouse.x", dummy_i);
  writeable.set("mouse.y", dummy_i);
  writeable.set("mouse.mask", dummy_i);

  return true;
}
void videoVNC::setProperties(gem::Properties&props)
{
  m_props=props;

  bool doMouse=false;

  double num;
  std::string s;
  if(props.get("password", s)) {
    m_password=s;
  }
  if(props.get("mouse.x", num)) {
    doMouse=true;
    m_mouse.x=num;
  }
  if(props.get("mouse.y", num)) {
    doMouse=true;
    m_mouse.y=num;
  }
  if(props.get("mouse.mask", num)) {
    doMouse=true;
    m_mouse.mask=num;
  }

  if(doMouse && m_client) {
    if ((m_mouse.x != -1) && (m_mouse.y != -1)) {
      SendPointerEvent  (m_client, m_mouse.x, m_mouse.y, m_mouse.mask);
      m_mouse.x = -1;
      m_mouse.y = -1;
    }
  }
}
void videoVNC::getProperties(gem::Properties&props)
{
  std::vector<std::string>keys=props.keys();
  for(unsigned int i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

std::vector<std::string>videoVNC::dialogs(void)
{
  std::vector<std::string>result;
  return result;
}
bool videoVNC::provides(const std::string&name)
{
  return (name==m_name);
}
std::vector<std::string>videoVNC::provides(void)
{
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoVNC::getName(void)
{
  return m_name;
}

void videoVNC::frameBufferCallback(rfbClient *client, int x, int y, int w,
                                   int h)
{
  int X,Y;
  int i,j;
  rfbPixelFormat* pf=&client->format;
  int bpp=pf->bitsPerPixel/8;
  int row_stride=client->width*bpp;

  m_pixBlock.image.setFormat(GEM_RGBA);
  m_pixBlock.image.xsize=client->width;
  m_pixBlock.image.ysize=client->height;
  m_pixBlock.image.reallocate();

  /* assert bpp=4 */
  if(bpp!=4 && bpp!=2) {
    return;
  }

  /* uncompress the framebuffer to our pixbuf */
  for(j=0,Y=0; j<client->height*row_stride; j+=row_stride,Y++) {
    for(i=0,X=0; i<client->width*bpp; i+=bpp,X++) {
      unsigned char* p=client->frameBuffer+j+i;
      unsigned int v=*(unsigned char*)p;
      switch(bpp) {
      case 4:
        v=*(unsigned int*)p;
        break;
      case 2:
        v=*(unsigned short*)p;
        break;
      default:
        break;
      }

      m_pixBlock.image.SetPixel(Y, X, chRed,
                                (v>>pf->redShift)  *256/(pf->redMax  +1));
      m_pixBlock.image.SetPixel(Y, X, chGreen,
                                (v>>pf->greenShift)*256/(pf->greenMax+1));
      m_pixBlock.image.SetPixel(Y, X, chBlue,
                                (v>>pf->blueShift) *256/(pf->blueMax+1));
    }
  }
  m_pixBlock.newimage = true;
}
char* videoVNC::passwordCallback()
{
  char*pwd=0;
  if(!m_password.empty()) {
    pwd=strdup(m_password.c_str());
  }
  return pwd;
}


void videoVNC::frameBufferCB(rfbClient *client, int x, int y, int w, int h)
{
  videoVNC*obj=rfb2gem(client);
  if(obj) {
    obj->frameBufferCallback(client,x,y,w,h);
  }
}
char*videoVNC::passwordCB(rfbClient*client)
{
  videoVNC*obj=rfb2gem(client);
  if(obj) {
    return obj->passwordCallback();
  }
  return 0;
}
