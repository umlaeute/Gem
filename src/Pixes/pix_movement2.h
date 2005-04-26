#ifndef INCLUDE_PIX_MOVEMENT2_H_
#define INCLUDE_PIX_MOVEMENT2_H_

#include "Base/GemPixObj.h"

class GEM_EXTERN pix_movement2 : public GemPixObj
{
  CPPEXTERN_HEADER(pix_movement2, GemPixObj)
		
    public:
  
  //////////
  // Constructor
  pix_movement2(t_float lotresh=0.392f, t_float hitresh=0.588f);
  
 protected:
  //Destructor
  ~pix_movement2();
  void processImage(imageStruct &image);

  imageStruct m_frame[3];
  imageStruct m_output, m_treshold, m_background;
  int m_frameIndex;

  unsigned char m_tresh, m_lowtresh;

  bool m_storeBackground, m_resetTreshold;

  t_inlet*m_treshInlet, *m_lowtreshInlet;


  void treshMess(int tresh);
  void lowTreshMess(int tresh);
  void bangMess();

 private:
  static void treshMessCallback(void *data, t_floatarg ftresh);
  static void lowTreshMessCallback(void *data, t_floatarg ftresh);
  static void bangMessCallback(void *data);
};


#endif 	// for header file
