// Copyright (c) 2004 Tim Blechmann
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "GEM.LICENSE.TERMS"  in this distribution.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// See file GEM.LICENSE.TERMS for further informations on licensing terms.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include "pix_mean_color.h"


CPPEXTERN_NEW_WITH_GIMME(pix_mean_color)

/* const, destructor */
pix_mean_color::pix_mean_color(int argc, t_atom *argv)
{
	m_list = outlet_new(this->x_obj, 0);
}

pix_mean_color::~pix_mean_color()
{
	outlet_free(m_list);
}

void pix_mean_color::processGrayImage(imageStruct &image)
{
	int datasize = image.xsize * image.ysize;
	int xsize = image.xsize;
	unsigned char *base = image.data;
	
	unsigned long sum = 0;

	for (int i = 0; i != datasize; ++i)
	{
		sum  += *base;
		base++;
	}
	
		
	SETFLOAT(out, (float)sum / (float)datasize );
	outlet_float(m_list, (float)sum / (float)datasize);
}

void pix_mean_color::processRGBImage(imageStruct &image)
{
	int datasize = image.xsize * image.ysize;
	int xsize = image.xsize;
	unsigned char *base = image.data;
	
	unsigned long sum[3] = {0,0,0};

	for (int i = 0; i != datasize; ++i)
	{
		for(int j = 0; j!= 3; ++j)
		{
			sum[j]  += base[j];
		}
		base += 3;
	}
	
	t_atom out[3];
	
	SETFLOAT(out, (float)sum[0] / (float)datasize );
	SETFLOAT(out+1, (float)sum[1] / (float)datasize );
	SETFLOAT(out+2, (float)sum[2] / (float)datasize );

	outlet_list(m_list, 0, 3, out);
}

void pix_mean_color::processRGBAImage(imageStruct &image)
{
	int datasize = image.xsize * image.ysize;
	int xsize = image.xsize;
	unsigned char *base = image.data;
	
	unsigned long sum[4] = {0,0,0,0};

	for (int i = 0; i != datasize; ++i)
	{
		for(int j = 0; j!= 4; ++j)
		{
			sum[j]  += base[j];
		}
		base += 4;
	}
	
	t_atom out[4];
	
	SETFLOAT(out, (float)sum[0] / (float)datasize );
	SETFLOAT(out+1, (float)sum[1] / (float)datasize );
	SETFLOAT(out+2, (float)sum[2] / (float)datasize );
	SETFLOAT(out+3, (float)sum[3] / (float)datasize );

	outlet_list(m_list, 0, 4, out);
}

void pix_mean_color :: obj_setupCallback(t_class *classPtr)
{

}
