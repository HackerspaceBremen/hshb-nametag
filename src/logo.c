#include "logo.h"

#include <avr/io.h>

extern uint8_t vRAM[522];

void logo_fill_percent(int8_t perc, uint8_t full, uint8_t onB, uint8_t offB) {
  uint8_t leds = (perc * full + 50) / 100;
  if (perc < 0) {
    leds = 0;
  } else if (perc > 100) {
    leds = full;
  }
  for (uint8_t i = 0; i <= 18; i++) {
    if (i < leds) {
      logo_draw_line(0, 18 - i, 14, 18 - i, onB);
    } else {
      logo_draw_line(0, 18 - i, 14, 18 - i, offB);
    }
  }
}

void logo_draw_line(int8_t x1, int8_t y1, int8_t x2, int8_t y2,
                    uint8_t brightness) {
  int8_t dx = x2 - x1;
  int8_t dy = y2 - y1;
  if (dx < 0) dx *= -1;
  if (dy < 0) dy *= -1;
  int8_t sx = x1 < x2 ? 1 : -1;
  int8_t sy = y1 < y2 ? 1 : -1;
  int8_t err = dx - dy;
  while (x1 != x2 || y1 != y2) {
    logo_set_xy(x1, y1, brightness);
    int8_t e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
  logo_set_xy(x2, y2, brightness);
}

void logo_draw_circle(int8_t x, int8_t y, int8_t r, uint8_t brightness) {
  int8_t x1 = 0;
  int8_t y1 = r;
  int8_t d = 3 - 2 * r;
  while (x1 <= y1) {
    logo_set_xy(x + x1, y + y1, brightness);
    logo_set_xy(x + x1, y - y1, brightness);
    logo_set_xy(x - x1, y + y1, brightness);
    logo_set_xy(x - x1, y - y1, brightness);
    logo_set_xy(x + y1, y + x1, brightness);
    logo_set_xy(x + y1, y - x1, brightness);
    logo_set_xy(x - y1, y + x1, brightness);
    logo_set_xy(x - y1, y - x1, brightness);
    x1++;
    if (d < 0) {
      d += 4 * x1 + 6;
    } else {
      y1--;
      d += 4 * (x1 - y1) + 10;
    }
  }
}

void logo_set_xy(uint8_t x, uint8_t y, uint8_t brightness) {
  if (x > 14 || y > 18) return;

  uint16_t pix = x + y * 15;

  switch (pix) {
    case (5):
      vRAM[420] = brightness;
      break;

    case (19):
      vRAM[421] = brightness;
      break;
    case (20):
      vRAM[422] = brightness;
      break;
    case (23):
      vRAM[423] = brightness;
      break;
    case (24):
      vRAM[424] = brightness;
      break;

    case (35):
      vRAM[425] = brightness;
      break;
    case (36):
      vRAM[426] = brightness;
      break;
    case (37):
      vRAM[427] = brightness;
      break;
    case (38):
      vRAM[428] = brightness;
      break;
    case (39):
      vRAM[429] = brightness;
      break;
    case (40):
      vRAM[430] = brightness;
      break;

    case (51):
      vRAM[431] = brightness;
      break;
    case (52):
      vRAM[432] = brightness;
      break;
    case (53):
      vRAM[433] = brightness;
      break;

    case (64):
      vRAM[434] = brightness;
      break;
    case (67):
      vRAM[435] = brightness;
      break;
    case (71):
      vRAM[436] = brightness;
      break;

    case (80):
      vRAM[437] = brightness;
      break;
    case (81):
      vRAM[438] = brightness;
      break;
    case (82):
      vRAM[439] = brightness;
      break;
    case (83):
      vRAM[440] = brightness;
      break;
    case (84):
      vRAM[441] = brightness;
      break;
    case (85):
      vRAM[442] = brightness;
      break;

    case (96):
      vRAM[443] = brightness;
      break;
    case (98):
      vRAM[444] = brightness;
      break;
    case (99):
      vRAM[445] = brightness;
      break;

    case (108):
      vRAM[446] = brightness;
      break;
    case (109):
      vRAM[447] = brightness;
      break;
    case (110):
      vRAM[448] = brightness;
      break;
    case (111):
      vRAM[449] = brightness;
      break;
    case (112):
      vRAM[450] = brightness;
      break;
    case (113):
      vRAM[451] = brightness;
      break;
    case (114):
      vRAM[452] = brightness;
      break;
    case (116):
      vRAM[453] = brightness;
      break;

    case (124):
      vRAM[454] = brightness;
      break;
    case (125):
      vRAM[455] = brightness;
      break;
    case (126):
      vRAM[456] = brightness;
      break;
    case (127):
      vRAM[457] = brightness;
      break;
    case (128):
      vRAM[458] = brightness;
      break;
    case (129):
      vRAM[459] = brightness;
      break;
    case (130):
      vRAM[460] = brightness;
      break;

    case (136):
      vRAM[461] = brightness;
      break;
    case (141):
      vRAM[462] = brightness;
      break;
    case (144):
      vRAM[463] = brightness;
      break;

    case (151):
      vRAM[464] = brightness;
      break;
    case (152):
      vRAM[465] = brightness;
      break;
    case (156):
      vRAM[466] = brightness;
      break;
    case (159):
      vRAM[467] = brightness;
      break;

    case (166):
      vRAM[468] = brightness;
      break;
    case (167):
      vRAM[469] = brightness;
      break;
    case (168):
      vRAM[470] = brightness;
      break;
    case (170):
      vRAM[471] = brightness;
      break;
    case (171):
      vRAM[472] = brightness;
      break;
    case (172):
      vRAM[473] = brightness;
      break;
    case (173):
      vRAM[474] = brightness;
      break;
    case (174):
      vRAM[475] = brightness;
      break;
    case (175):
      vRAM[476] = brightness;
      break;
    case (176):
      vRAM[477] = brightness;
      break;

    case (180):
      vRAM[478] = brightness;
      break;
    case (181):
      vRAM[479] = brightness;
      break;
    case (182):
      vRAM[480] = brightness;
      break;
    case (183):
      vRAM[481] = brightness;
      break;
    case (184):
      vRAM[482] = brightness;
      break;
    case (185):
      vRAM[483] = brightness;
      break;
    case (186):
      vRAM[484] = brightness;
      break;
    case (187):
      vRAM[485] = brightness;
      break;
    case (188):
      vRAM[486] = brightness;
      break;
    case (189):
      vRAM[487] = brightness;
      break;
    case (190):
      vRAM[488] = brightness;
      break;
    case (191):
      vRAM[489] = brightness;
      break;
    case (192):
      vRAM[490] = brightness;
      break;

    case (195):
      vRAM[491] = brightness;
      break;
    case (196):
      vRAM[492] = brightness;
      break;
    case (199):
      vRAM[493] = brightness;
      break;
    case (200):
      vRAM[494] = brightness;
      break;
    case (201):
      vRAM[495] = brightness;
      break;
    case (202):
      vRAM[496] = brightness;
      break;
    case (203):
      vRAM[497] = brightness;
      break;
    case (204):
      vRAM[498] = brightness;
      break;
    case (205):
      vRAM[499] = brightness;
      break;
    case (206):
      vRAM[500] = brightness;
      break;
    case (207):
      vRAM[501] = brightness;
      break;
    case (208):
      vRAM[502] = brightness;
      break;

    case (214):
      vRAM[503] = brightness;
      break;
    case (215):
      vRAM[504] = brightness;
      break;
    case (216):
      vRAM[505] = brightness;
      break;
    case (217):
      vRAM[506] = brightness;
      break;
    case (218):
      vRAM[507] = brightness;
      break;
    case (221):
      vRAM[508] = brightness;
      break;
    case (222):
      vRAM[509] = brightness;
      break;
    case (223):
      vRAM[510] = brightness;
      break;

    case (229):
      vRAM[511] = brightness;
      break;
    case (230):
      vRAM[512] = brightness;
      break;
    case (237):
      vRAM[513] = brightness;
      break;
    case (239):
      vRAM[514] = brightness;
      break;

    case (244):
      vRAM[515] = brightness;
      break;
    case (252):
      vRAM[516] = brightness;
      break;

    case (258):
      vRAM[517] = brightness;
      break;
    case (267):
      vRAM[518] = brightness;
      break;

    case (272):
      vRAM[519] = brightness;
      break;
    case (273):
      vRAM[520] = brightness;
      break;
    case (281):
      vRAM[521] = brightness;
      break;

    default:
      break;
  }
}
