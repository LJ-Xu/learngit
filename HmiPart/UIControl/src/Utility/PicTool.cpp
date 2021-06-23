#include <FL/Fl_BMP_Image.H>
#include <FL/fl_utf8.h>
#include <FL/Fl.H>
#include <stdio.h>
#include <stdlib.h>
#include "PicTool.h"
namespace UI
{
	//
	// BMP definitions...
	//

#ifndef BI_RGB
#  define BI_RGB       0             // No compression - straight BGR data
#  define BI_RLE8      1             // 8-bit run-length compression
#  define BI_RLE4      2             // 4-bit run-length compression
#  define BI_BITFIELDS 3             // RGB bitmap with RGB masks
#endif // !BI_RGB

//
// Local functions...
//
	static inline int getchar(const char* *ptr, const char* end)
	{
		//TODO 验证是否超出文件大小
		return (unsigned char)*(*ptr)++;
	}
	static int read_long(const char* *ptr, const char* end)
	{
		unsigned char b0, b1, b2, b3;	// Bytes from file

		b0 = (uchar)getchar(ptr,end);
		b1 = (uchar)getchar(ptr, end);
		b2 = (uchar)getchar(ptr, end);
		b3 = (uchar)getchar(ptr, end);

		return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
		//return ((int)((((((*ptr)[3] << 8) | (*ptr)[2]) << 8) | (*ptr)[1]) << 8) | (*ptr)[0]);
	}
	static unsigned short read_word(const char* *ptr, const char* end)
	{
		//TODO 验证是否超出文件大小
		//*ptr += 2;
		//return (unsigned short)((((*ptr)[1]) << 8) | (*ptr)[0]);
		unsigned char b0, b1;	// Bytes from file

		b0 = (uchar)getchar(ptr,end);
		b1 = (uchar)getchar(ptr, end);

		return ((b1 << 8) | b0);
	}
	static unsigned int	read_dword(const char* *ptr, const char* end)
	{
		//TODO 验证是否超出文件大小
		unsigned char b0, b1, b2, b3;	// Bytes from file

		b0 = (uchar)getchar(ptr, end);
		b1 = (uchar)getchar(ptr, end);
		b2 = (uchar)getchar(ptr, end);
		b3 = (uchar)getchar(ptr, end);

		return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
		//*ptr += 4;
		//return ((unsigned int)((((((*ptr)[3] << 8) | (*ptr)[2]) << 8) | (*ptr)[1]) << 8) | (*ptr)[0]);
	}
	static int	picread(uchar* buf, int itemsize, int count, const char* *ptr, const char* end)
	{
		//TODO 验证是否超出文件大小
		//*ptr += itemsize * count;
		memcpy(buf, *ptr, itemsize*count);
		*ptr += itemsize * count;
		return 0;
	}

	/**
	 The constructor loads the named BMP image from the given bmp filename.

	 The destructor frees all memory and server resources that are used by
	 the image.

	 Use Fl_Image::fail() to check if Fl_BMP_Image failed to load. fail() returns
	 ERR_FILE_ACCESS if the file could not be opened or read, ERR_FORMAT if the
	 BMP format could not be decoded, and ERR_NO_IMAGE if the image could not
	 be loaded for another reason.
	 */
	BinBitmap::BinBitmap(const char *data, int size) // I - File to read
		: Fl_RGB_Image(0, 0, 0) {
		int		info_size,	// Size of info header
			depth,		// Depth of image (bits)
			bDepth = 3,	// Depth of image (bytes)
			compression,	// Type of compression
			colors_used,	// Number of colors used
			x, y,		// Looping vars
			color,		// Color of RLE pixel
			repcount,	// Number of times to repeat
			temp,		// Temporary color
			align,		// Alignment bytes
			dataSize,	// number of bytes in image data set
			row_order,	// 1 = normal;  -1 = flipped row order
			start_y,	// Beginning Y
			end_y;		// Ending Y
		long		offbits;	// Offset to image data
		uchar		bit,		// Bit in image
			byte;		// Byte in image
		uchar		*ptr;		// Pointer into pixels
		uchar		colormap[256][3];// Colormap
		uchar		havemask;	// Single bit mask follows image data
		int		use_5_6_5;	// Use 5:6:5 for R:G:B channels in 16 bit images
		const char* pdata = data;
		const char* pend = data + size;
		// Get the header...
		byte = (uchar)getchar(&pdata, pend);	// Check "BM" sync chars
		bit = (uchar)getchar(&pdata, pend);
		if (byte != 'B' || bit != 'M') {
			ld(ERR_FORMAT);
			return;
		}

		read_dword(&pdata, pend);		// Skip size
		read_word(&pdata, pend);		// Skip reserved stuff
		read_word(&pdata, pend);
		offbits = (long)read_dword(&pdata, pend);// Read offset to image data

		// Then the bitmap information...
		info_size = read_dword(&pdata, pend);

		//  printf("offbits = %ld, info_size = %d\n", offbits, info_size);

		havemask = 0;
		row_order = -1;
		use_5_6_5 = 0;

		if (info_size < 40) {
			// Old Windows/OS2 BMP header...
			w(read_word(&pdata, pend));
			h(read_word(&pdata, pend));
			read_word(&pdata, pend);
			depth = read_word(&pdata, pend);
			compression = BI_RGB;
			colors_used = 0;

			repcount = info_size - 12;
		}
		else {
			// New BMP header...
			w(read_long(&pdata, pend));
			// If the height is negative, the row order is flipped
			temp = read_long(&pdata, pend);
			if (temp < 0) row_order = 1;
			h(abs(temp));
			read_word(&pdata, pend);
			depth = read_word(&pdata, pend);
			compression = read_dword(&pdata, pend);
			dataSize = read_dword(&pdata, pend);
			read_long(&pdata, pend);
			read_long(&pdata, pend);
			colors_used = read_dword(&pdata, pend);
			read_dword(&pdata, pend);

			repcount = info_size - 40;

			if (!compression && depth >= 8 && w() > 32 / depth) {
				int Bpp = depth / 8;
				int maskSize = (((w()*Bpp + 3)&~3)*h()) + (((((w() + 7) / 8) + 3)&~3)*h());
				if (maskSize == 2 * dataSize) {
					havemask = 1;
					h(h() / 2);
					bDepth = 4;
				}
			}
		}

		//  printf("w() = %d, h() = %d, depth = %d, compression = %d, colors_used = %d, repcount = %d\n",
		//         w(), h(), depth, compression, colors_used, repcount);

		  // Skip remaining header bytes...
		while (repcount > 0) {
			getchar(&pdata, pend);
			repcount--;
		}

		// Check header data...
		if (!w() || !h() || !depth) {
			w(0); h(0); d(0); ld(ERR_FORMAT);
			return;
		}

		// Get colormap...
		if (colors_used == 0 && depth <= 8)
			colors_used = 1 << depth;

		for (repcount = 0; repcount < colors_used; repcount++) {
			// Read BGR color...
			if (picread(colormap[repcount], 1, 3, &pdata, pend) == 0) { /* ignore */ }
			// Skip pad byte for new BMP files...
			if (info_size > 12) getchar(&pdata, pend);
		}

		// Read first dword of colormap. It tells us if 5:5:5 or 5:6:5 for 16 bit
		if (depth == 16)
			use_5_6_5 = (read_dword(&pdata, pend) == 0xf800);

		// Set byte depth for RGBA images
		if (depth == 32)
			bDepth = 4;

		// Setup image and buffers...
		d(bDepth);
		if (offbits)
		{
			pdata = data + offbits;
			//fseek(fp, offbits, SEEK_SET);
		}

		if (((size_t)w()) * h() * d() > max_size()) {
			Fl::warning("BMP file is too large!\n");
			w(0); h(0); d(0); ld(ERR_FORMAT);
			return;
		}
		array = new uchar[w() * h() * d()];
		alloc_array = 1;

		// Read the image data...
		color = 0;
		repcount = 0;
		align = 0;
		byte = 0;
		temp = 0;

		if (row_order < 0) {
			start_y = h() - 1;
			end_y = -1;
		}
		else {
			start_y = 0;
			end_y = h();
		}

		for (y = start_y; y != end_y; y += row_order) {
			ptr = (uchar *)array + y * w() * d();

			switch (depth)
			{
			case 1: // Bitmap
				for (x = w(), bit = 128; x > 0; x--) {
					if (bit == 128) byte = (uchar)getchar(&pdata, pend);

					if (byte & bit) {
						*ptr++ = colormap[1][2];
						*ptr++ = colormap[1][1];
						*ptr++ = colormap[1][0];
					}
					else {
						*ptr++ = colormap[0][2];
						*ptr++ = colormap[0][1];
						*ptr++ = colormap[0][0];
					}

					if (bit > 1)
						bit >>= 1;
					else
						bit = 128;
				}

				// Read remaining bytes to align to 32 bits...
				for (temp = (w() + 7) / 8; temp & 3; temp++) {
					getchar(&pdata, pend);
				}
				break;

			case 4: // 16-color
				for (x = w(), bit = 0xf0; x > 0; x--) {
					// Get a new repcount as needed...
					if (repcount == 0) {
						if (compression != BI_RLE4) {
							repcount = 2;
							color = -1;
						}
						else {
							while (align > 0) {
								align--;
								getchar(&pdata, pend);
							}

							if ((repcount = getchar(&pdata, pend)) == 0) {
								if ((repcount = getchar(&pdata, pend)) == 0) {
									// End of line...
									x++;
									continue;
								}
								else if (repcount == 1) {
									// End of image...
									break;
								}
								else if (repcount == 2) {
									// Delta...
									repcount = getchar(&pdata, pend) * getchar(&pdata, pend) * w();
									color = 0;
								}
								else {
									// Absolute...
									color = -1;
									align = ((4 - (repcount & 3)) / 2) & 1;
								}
							}
							else {
								color = getchar(&pdata, pend);
							}
						}
					}

					// Get a new color as needed...
					repcount--;

					// Extract the next pixel...
					if (bit == 0xf0) {
						// Get the next color byte as needed...
						if (color < 0) temp = getchar(&pdata, pend);
						else temp = color;

						// Copy the color value...
						*ptr++ = colormap[(temp >> 4) & 15][2];
						*ptr++ = colormap[(temp >> 4) & 15][1];
						*ptr++ = colormap[(temp >> 4) & 15][0];

						bit = 0x0f;
					}
					else {
						bit = 0xf0;

						// Copy the color value...
						*ptr++ = colormap[temp & 15][2];
						*ptr++ = colormap[temp & 15][1];
						*ptr++ = colormap[temp & 15][0];
					}

				}

				if (!compression) {
					// Read remaining bytes to align to 32 bits...
					for (temp = (w() + 1) / 2; temp & 3; temp++) {
						getchar(&pdata, pend);
					}
				}
				break;

			case 8: // 256-color
				for (x = w(); x > 0; x--) {
					// Get a new repcount as needed...
					if (compression != BI_RLE8) {
						repcount = 1;
						color = -1;
					}

					if (repcount == 0) {
						while (align > 0) {
							align--;
							getchar(&pdata, pend);
						}

						if ((repcount = getchar(&pdata, pend)) == 0) {
							if ((repcount = getchar(&pdata, pend)) == 0) {
								// End of line...
								x++;
								continue;
							}
							else if (repcount == 1) {
								// End of image...
								break;
							}
							else if (repcount == 2) {
								// Delta...
								repcount = getchar(&pdata, pend) * getchar(&pdata, pend) * w();
								color = 0;
							}
							else {
								// Absolute...
								color = -1;
								align = (2 - (repcount & 1)) & 1;
							}
						}
						else {
							color = getchar(&pdata, pend);
						}
					}

					// Get a new color as needed...
					if (color < 0) temp = getchar(&pdata, pend);
					else temp = color;

					repcount--;

					// Copy the color value...
					*ptr++ = colormap[temp][2];
					*ptr++ = colormap[temp][1];
					*ptr++ = colormap[temp][0];
					if (havemask) ptr++;
				}

				if (!compression) {
					// Read remaining bytes to align to 32 bits...
					for (temp = w(); temp & 3; temp++) {
						getchar(&pdata, pend);
					}
				}
				break;

			case 16: // 16-bit 5:5:5 or 5:6:5 RGB
				for (x = w(); x > 0; x--, ptr += bDepth) {
					uchar b = getchar(&pdata, pend), a = getchar(&pdata, pend);
					if (use_5_6_5) {
						ptr[2] = (uchar)((b << 3) & 0xf8);
						ptr[1] = (uchar)(((a << 5) & 0xe0) | ((b >> 3) & 0x1c));
						ptr[0] = (uchar)(a & 0xf8);
					}
					else {
						ptr[2] = (uchar)((b << 3) & 0xf8);
						ptr[1] = (uchar)(((a << 6) & 0xc0) | ((b >> 2) & 0x38));
						ptr[0] = (uchar)((a << 1) & 0xf8);
					}
				}

				// Read remaining bytes to align to 32 bits...
				for (temp = w() * 2; temp & 3; temp++) {
					getchar(&pdata, pend);
				}
				break;

			case 24: // 24-bit RGB
				for (x = w(); x > 0; x--, ptr += bDepth) {
					ptr[2] = (uchar)getchar(&pdata, pend);
					ptr[1] = (uchar)getchar(&pdata, pend);
					ptr[0] = (uchar)getchar(&pdata, pend);
				}

				// Read remaining bytes to align to 32 bits...
				for (temp = w() * 3; temp & 3; temp++) {
					getchar(&pdata, pend);
				}
				break;

			case 32: // 32-bit RGBA
				for (x = w(); x > 0; x--, ptr += bDepth) {
					ptr[2] = (uchar)getchar(&pdata, pend);
					ptr[1] = (uchar)getchar(&pdata, pend);
					ptr[0] = (uchar)getchar(&pdata, pend);
					ptr[3] = (uchar)getchar(&pdata, pend);
				}
				break;
			}
		}
		if (havemask) {
			for (y = h() - 1; y >= 0; y--) {
				ptr = (uchar *)array + y * w() * d() + 3;
				for (x = w(), bit = 128; x > 0; x--, ptr += bDepth) {
					if (bit == 128) byte = (uchar)getchar(&pdata, pend);
					if (byte & bit)
						*ptr = 0;
					else
						*ptr = 255;
					if (bit > 1)
						bit >>= 1;
					else
						bit = 128;
				}
				// Read remaining bytes to align to 32 bits...
				for (temp = (w() + 7) / 8; temp & 3; temp++)
					getchar(&pdata, pend);
			}
		}


	}
}