#ifndef __EF_IMAGE_FILES_H__
#define __EF_IMAGE_FILES_H__

#include <ef/image.h>

/** load image, supported png, jpeg, svg, bmp, gif only with g2d_load_gif
 * @param path absolute path
 * @param width if 0 no scaling
 * @param height if 0 no scaling
 * @return new image or null for error
 */
g2dImage_s* g2d_load(char const* path, unsigned width, unsigned height);



#endif 
