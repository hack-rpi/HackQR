#include "qr.h"

const util::GaloisField QR::GF_ = util::GaloisField(256, 285);

const util::Binary QR::PAD_BYTES1_ = util::Binary("11101100");

const util::Binary QR::PAD_BYTES2_ = util::Binary("00010001");

const std::map<std::string, std::string> QR::EC_CODEWORDS_ = {
  {"1-L",	"19:7:1:19:0:0"},
  {"1-M",	"16:10:1:16:0:0"},
  {"1-Q",	"13:13:1:13:0:0"},
  {"1-H",	"9:17:1:9:0:0"},
  {"2-L",	"34:10:1:34:0:0"},
  {"2-M",	"28:16:1:28:0:0"},
  {"2-Q", "22:22:1:22:0:0"},
  {"2-H", "16:28:1:16:0:0"},
  {"3-L", "55:15:1:55:0:0"},
  {"3-M", "44:26:1:44:0:0"},
  {"3-Q", "34:18:2:17:0:0"},
  {"3-H", "26:22:2:13:0:0"},
  {"4-L", "80:20:1:80:0:0"},
  {"4-M", "64:18:2:32:0:0"},
  {"4-Q", "48:26:2:24:0:0"},
  {"4-H",	"36:16:4:9:0:0"},
  {"5-L", "108:26:1:108:0:0"},
  {"5-M", "86:24:2:43:0:0"},
  {"5-Q", "62:18:2:15:2:16"},
  {"5-H", "46:22:2:11:2:12"},
  {"6-L", "136:18:2:68:0:0"},
  {"6-M", "108:16:4:27:0:0"},
  {"6-Q", "76:24:4:19:0:0"},
  {"6-H", "60:28:4:15:0:0"},
  {"7-L", "156:20:2:78:0:0"},
  {"7-M", "124:18:4:31:0:0"},
  {"7-Q", "88:18:2:14:4:15"},
  {"7-H", "66:26:4:13:1:14"},
  {"8-L", "194:24:2:97:0:0"},
  {"8-M", "154:22:2:38:2:39"},
  {"8-Q", "110:22:4:18:2:19"},
  {"8-H", "86:26:4:14:2:15"},
  {"9-L", "232:30:2:116:0:0"},
  {"9-M", "182:22:3:36:2:37"},
  {"9-Q", "132:20:4:16:4:17"},
  {"9-H", "100:24:4:12:4:13"},
  {"10-L", "274:18:2:68:2:69"},
  {"10-M", "216:26:4:43:1:44"},
  {"10-Q", "154:24:6:19:2:20"},
  {"10-H", "122:28:6:15:2:16"},
  {"11-L", "324:20:4:81:0:0"},
  {"11-M", "254:30:1:50:4:51"},
  {"11-Q", "180:28:4:22:4:23"},
  {"11-H", "140:24:3:12:8:13"},
  {"12-L", "370:24:2:92:2:93"},
  {"12-M", "290:22:6:36:2:37"},
  {"12-Q", "206:26:4:20:6:21"},
  {"12-H", "158:28:7:14:4:15"},
  {"13-L", "428:26:4:107:0:0"},
  {"13-M", "334:22:8:37:1:38"},
  {"13-Q", "244:24:8:20:4:21"},
  {"13-H", "180:22:12:11:4:12"},
  {"14-L", "461:30:3:115:1:116"},
  {"14-M", "365:24:4:40:5:41"},
  {"14-Q", "261:20:11:16:5:17"},
  {"14-H", "197:24:11:12:5:13"},
  {"15-L", "523:22:5:87:1:88"},
  {"15-M", "415:24:5:41:5:42"},
  {"15-Q", "295:30:5:24:7:25"},
  {"15-H", "223:24:11:12:7:13"},
  {"16-L", "589:24:5:98:1:99"},
  {"16-M", "453:28:7:45:3:46"},
  {"16-Q", "325:24:15:19:2:20"},
  {"16-H", "253:30:3:15:13:16"},
  {"17-L", "647:28:1:107:5:108"},
  {"17-M", "507:28:10:46:1:47"},
  {"17-Q", "367:28:1:22:15:23"},
  {"17-H", "283:28:2:14:17:15"},
  {"18-L", "721:30:5:120:1:121"},
  {"18-M", "563:26:9:43:4:44"},
  {"18-Q", "397:28:17:22:1:23"},
  {"18-H", "313:28:2:14:19:15"},
  {"19-L", "795:28:3:113:4:114"},
  {"19-M", "627:26:3:44:11:45"},
  {"19-Q", "445:26:17:21:4:22"},
  {"19-H", "341:26:9:13:16:14"},
  {"20-L", "861:28:3:107:5:108"},
  {"20-M", "669:26:3:41:13:42"},
  {"20-Q", "485:30:15:24:5:25"},
  {"20-H", "385:28:15:15:10:16"},
  {"21-L", "932:28:4:116:4:117"},
  {"21-M", "714:26:17:42:0:0"},
  {"21-Q", "512:28:17:22:6:23"},
  {"21-H", "406:30:19:16:6:17"},
  {"22-L", "1006:28:2:111:7:112"},
  {"22-M", "782:28:17:46:0:0"},
  {"22-Q", "568:30:7:24:16:25"},
  {"22-H", "442:24:34:13:0:0"},
  {"23-L", "1094:30:4:121:5:122"},
  {"23-M", "860:28:4:47:14:48"},
  {"23-Q", "614:30:11:24:14:25"},
  {"23-H", "464:30:16:15:14:16"},
  {"24-L", "1174:30:6:117:4:118"},
  {"24-M", "914:28:6:45:14:46"},
  {"24-Q", "664:30:11:24:16:25"},
  {"24-H", "514:30:30:16:2:17"},
  {"25-L", "1276:26:8:106:4:107"},
  {"25-M", "1000:28:8:47:13:48"},
  {"25-Q", "718:30:7:24:22:25"},
  {"25-H", "538:30:22:15:13:16"},
  {"26-L", "1370:28:10:114:2:115"},
  {"26-M", "1062:28:19:46:4:47"},
  {"26-Q", "754:28:28:22:6:23"},
  {"26-H", "596:30:33:16:4:17"},
  {"27-L", "1468:30:8:122:4:123"},
  {"27-M", "1128:28:22:45:3:46"},
  {"27-Q", "808:30:8:23:26:24"},
  {"27-H", "628:30:12:15:28:16"},
  {"28-L", "1531:30:3:117:10:118"},
  {"28-M", "1193:28:3:45:23:46"},
  {"28-Q", "871:30:4:24:31:25"},
  {"28-H", "661:30:11:15:31:16"},
  {"29-L", "1631:30:7:116:7:117"},
  {"29-M", "1267:28:21:45:7:46"},
  {"29-Q", "911:30:1:23:37:24"},
  {"29-H", "701:30:19:15:26:16"},
  {"30-L", "1735:30:5:115:10:116"},
  {"30-M", "1373:28:19:47:10:48"},
  {"30-Q", "985:30:15:24:25:25"},
  {"30-H", "745:30:23:15:25:16"},
  {"31-L", "1843:30:13:115:3:116"},
  {"31-M", "1455:28:2:46:29:47"},
  {"31-Q", "1033:30:42:24:1:25"},
  {"31-H", "793:30:23:15:28:16"},
  {"32-L", "1955:30:17:115:0:0"},
  {"32-M", "1541:28:10:46:23:47"},
  {"32-Q", "1115:30:10:24:35:25"},
  {"32-H", "845:30:19:15:35:16"},
  {"33-L", "2071:30:17:115:1:116"},
  {"33-M", "1631:28:14:46:21:47"},
  {"33-Q", "1171:30:29:24:19:25"},
  {"33-H", "901:30:11:15:46:16"},
  {"34-L", "2191:30:13:115:6:116"},
  {"34-M", "1725:28:14:46:23:47"},
  {"34-Q", "1231:30:44:24:7:25"},
  {"34-H", "961:30:59:16:1:17"},
  {"35-L", "2306:30:12:121:7:122"},
  {"35-M", "1812:28:12:47:26:48"},
  {"35-Q", "1286:30:39:24:14:25"},
  {"35-H", "986:30:22:15:41:16"},
  {"36-L", "2434:30:6:121:14:122"},
  {"36-M", "1914:28:6:47:34:48"},
  {"36-Q", "1354:30:46:24:10:25"},
  {"36-H", "1054:30:2:15:64:16"},
  {"37-L", "2566:30:17:122:4:123"},
  {"37-M", "1992:28:29:46:14:47"},
  {"37-Q", "1426:30:49:24:10:25"},
  {"37-H", "1096:30:24:15:46:16"},
  {"38-L", "2702:30:4:122:18:123"},
  {"38-M", "2102:28:13:46:32:47"},
  {"38-Q", "1502:30:48:24:14:25"},
  {"38-H", "1142:30:42:15:32:16"},
  {"39-L", "2812:30:20:117:4:118"},
  {"39-M", "2216:28:40:47:7:48"},
  {"39-Q", "1582:30:43:24:22:25"},
  {"39-H", "1222:30:10:15:67:16"},
  {"40-L", "2956:30:19:118:6:119"},
  {"40-M", "2334:28:18:47:31:48"},
  {"40-Q", "1666:30:34:24:34:25"},
  {"40-H", "1276:30:20:15:61:16"}
};

const std::map<int, size_t> QR::REMAINDER_BITS_ = {
  {1, 0},
  {2, 7},
  {3, 7},
  {4, 7},
  {5, 7},
  {6, 7},
  {7, 0},
  {8, 0},
  {9, 0},
  {10, 0},
  {11, 0},
  {12, 0},
  {13, 0},
  {14, 3},
  {15, 3},
  {16, 3},
  {17, 3},
  {18, 3},
  {19, 3},
  {20, 3},
  {21, 4},
  {22, 4},
  {23, 4},
  {24, 4},
  {25, 4},
  {26, 4},
  {27, 4},
  {28, 3},
  {29, 3},
  {30, 3},
  {31, 3},
  {32, 3},
  {33, 3},
  {34, 3},
  {35, 0},
  {36, 0},
  {37, 0},
  {38, 0},
  {39, 0},
  {40, 0}
};
