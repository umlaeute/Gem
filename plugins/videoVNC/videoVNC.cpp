#include "videoVNC.h"
#include "plugins/PluginFactory.h"

#include <m_pd.h>

using namespace gem::plugins;

REGISTER_VIDEOFACTORY("vnc", videoVNC);

static double getRandom(void) {
  static unsigned int random_nextseed = 1489853723;
  random_nextseed = random_nextseed * 435898247 + 938284281;
  return random_nextseed * (1./4294967296.);;
}

static videoVNC*rfb2gem(rfbClient*client) {
  return (videoVNC*)rfbClientGetClientData(client, (void*)(rfb2gem));
}



videoVNC::videoVNC(void)
  : m_name(std::string("vnc"))
  , m_client(0)
{
  m_pixBlock.image.xsize = 64;
  m_pixBlock.image.ysize = 64;
  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.reallocate();
}

videoVNC::~videoVNC(void) {
  close();
}

void videoVNC::close(void) {
  if(m_client) {
    rfbClientCleanup(m_client);
    m_client=0;
  }
}

bool videoVNC::open(gem::Properties&props) {
  setProperties(props);
  if(m_client)close();
  m_client=rfbGetClient(8,3,4);
  if(!m_client) {
    return false;
  }

  if(true) {
    int argc=1;
    char*argv="localhost";
    post("opening...");
    if(!rfbInitClient(m_client, &argc, &argv))
      return false;
  }
  post("done");
  return true;
}

pixBlock*videoVNC::getFrame(void) {
  if(m_client) {
    // process all RFB-messages so far...
    int w=WaitForMessage(m_client, 500);
    if(w>0) {
      if(!HandleRFBServerMessage(m_client)) {
	//return 0;
      }
    }
  }
  return &m_pixBlock;
}

std::vector<std::string>videoVNC::enumerate(void) {
  std::vector<std::string>result;
  result.push_back("vnc");
  return result;
}

bool videoVNC::setDevice(int ID) {
  m_devname.clear();
  return false;
}
bool videoVNC::setDevice(std::string device) {
  m_devname.clear();
  const std::string prefix="vnc://";
  if (!device.compare(0, prefix.size(), prefix)) {
    m_devname=device.substr(prefix.size());
    post("VNC: device '%s'", m_devname.c_str());
    return true;
  }
  return false;
}
bool videoVNC::enumProperties(gem::Properties&readable,
			       gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  readable.set("width", 64);
  readable.set("height", 64);

  return true;
}
void videoVNC::setProperties(gem::Properties&props) {
  m_props=props;
}
void videoVNC::getProperties(gem::Properties&props) {
  std::vector<std::string>keys=props.keys();
  unsigned int i;
  for(i=0; i<keys.size(); i++) {
    if("width"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.xsize);
    }
    if("height"==keys[i]) {
      props.set(keys[i], m_pixBlock.image.ysize);
    }
  }
}

std::vector<std::string>videoVNC::dialogs(void) {
  std::vector<std::string>result;
  return result;
}
bool videoVNC::provides(const std::string name) {
  return (name==m_name);
}
std::vector<std::string>videoVNC::provides(void) {
  std::vector<std::string>result;
  result.push_back(m_name);
  return result;
}
const std::string videoVNC::getName(void) {
  return m_name;
}

void videoVNC::frameBufferUpdated(rfbClient *client, int x, int y, int w, int h) {
  int X,Y;
  int i,j;
  rfbPixelFormat* pf=&client->format;
  int bpp=pf->bitsPerPixel/8;
  int row_stride=client->width*bpp;

  m_pixBlock.image.setCsizeByFormat(GL_RGBA);
  m_pixBlock.image.xsize=client->width;
  m_pixBlock.image.ysize=client->height;
  m_pixBlock.image.reallocate();

  /* assert bpp=4 */
  if(bpp!=4 && bpp!=2) { return; }

  /* uncompress the framebuffer to our pixbuf */
  for(j=0,Y=0;j<client->height*row_stride;j+=row_stride,Y++) {
    for(i=0,X=0;i<client->width*bpp;i+=bpp,X++) {
      unsigned char* p=client->frameBuffer+j+i;
      unsigned int v;
      if(bpp==4)
	v=*(unsigned int*)p;
      else if(bpp==2)
	v=*(unsigned short*)p;
      else
	v=*(unsigned char*)p;

      m_pixBlock.image.SetPixel(Y, X, chRed,  (v>>pf->redShift)  *256/(pf->redMax  +1));
      m_pixBlock.image.SetPixel(Y, X, chGreen,(v>>pf->greenShift)*256/(pf->greenMax+1));
      m_pixBlock.image.SetPixel(Y, X, chBlue, (v>>pf->blueShift) *256/(pf->blueMax+1));
    }
  }
  m_pixBlock.newimage = true;
}

void videoVNC::frameBufferCallback(rfbClient *client, int x, int y, int w, int h) {
  videoVNC*obj=(videoVNC*)rfbClientGetClientData(client, (void*)(frameBufferCallback));
  obj->frameBufferUpdated(client,x,y,w,h);
}
