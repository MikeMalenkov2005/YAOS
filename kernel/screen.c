#include "screen.h"

#define ABS(n) ((n) < 0 ? -(n) : (n))

static int stripes[] = {
  COLOR(255, 255, 255),
  COLOR(255, 255, 0),
  COLOR(0, 255, 255),
  COLOR(0, 255, 0),
  COLOR(255, 0, 255),
  COLOR(255, 0, 0),
  COLOR(0, 0, 255),
  COLOR(0, 0, 0)
};

int test_pattern(int x, int y, int w, int h, int th) {
  if (x < th || y < th || x >= w - th || y >= h - th) return COLOR(0, 255, 0);
  int cx = w >> 1;
  int cy = h >> 1;
  int or = cx > cy ? cy : cx;
  int ir = or - (th << 1);
  int dx = x - cx + (x < cx);
  int dy = y - cy + (y < cy);
  int dd = dx * dx + dy * dy;
  int si = (((dx + or + (x >= cx)) << 2) / (or + 1)) & 7;
  if (dd < ir * ir) return stripes[si];
  if (dd < or * or) return (si & 1) ? COLOR(255, 255, 255) : COLOR(127, 127, 127);
  if ((ABS(dx) % (or >> 1)) < th || (ABS(dy) % (or >> 1)) < th) return COLOR(255, 0, 0);
  return COLOR(0, 0, 0);
}

void plot_pixel(int x, int y, int color) {
  unsigned char* buffer = _get_screen_buffer();
  int bytes = (_get_screen_depth() + 7) >> 3;
  int index = x * bytes + y * _get_screen_pitch();
  for (int i = 0; i < bytes; ++i) {
    buffer[index + i] = (color >> (i << 3)) & 255;
  }
}

void draw_screen() {
  int w = _get_screen_width();
  int h = _get_screen_height();
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      plot_pixel(x, y, _get_screen_color(test_pattern(x, y, w, h, 4)));
    }
  }
}

