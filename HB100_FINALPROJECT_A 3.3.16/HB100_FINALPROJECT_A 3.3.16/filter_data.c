/**
 *
 *
 **/

#include "arm_math.h"
#include "sampling.h"
#include "signal_dsp.h"
/* ----------------------------------------------------------------------
 ** Test input signal contains 1000Hz + 15000 Hz
 ** ------------------------------------------------------------------- */

const float32_t firCoeffs32[NUM_TAPS] = { -0.000074029787723175851,
		0.000017335114024242748, 0.000016714489931227413,
		0.000016914664698078393, 0.000017833760715256158,
		0.00001936753284143648, 0.000021470175773501382, 0.0000240808761912665,
		0.000027190109935793652, 0.00003076222082586599,
		0.000034808770994987535, 0.000039309720691864923,
		0.000044291245517772101, 0.000049739932687524062,
		0.000055692601758028553, 0.000062140388562404174,
		0.000069128401103763559, 0.000076645878781258207,
		0.000084749185850655908, 0.000093420483263056495,
		0.00010273336557745961, 0.00011265121488255643, 0.0001232480108380004,
		0.00013459636946903071, 0.00014650139507745382, 0.00015929463718936134,
		0.00017284015891234515, 0.00018715745983810908, 0.00020226733820720105,
		0.0002182498128765064, 0.00023512578116460171, 0.00025292651954488888,
		0.00027165870771194241, 0.00029134995579086707, 0.00031202217045904067,
		0.00033370999234300623, 0.00035644549456481851, 0.00038025899861406803,
		0.00040518081307119023, 0.0004312340532734016, 0.0004584503987046613,
		0.00048684430822789812, 0.00051645354569522907, 0.0005472828402515276,
		0.00057938190608542079, 0.00061275090975397421, 0.0006474127548303588,
		0.00068343468905098371, 0.00072076580050502625, 0.00075947631501319265,
		0.00079958110240744553, 0.0008410975840678226, 0.00088402254995322081,
		0.00092838867936580823, 0.00097421127156387751, 0.0010215107156041454,
		0.0010702884133779744, 0.0011205575804490652, 0.0011723248197248871,
		0.0012256044570541449, 0.0012804046232005564, 0.0013367332506084143,
		0.0013945919856421184, 0.0014539800651395143, 0.001514901520821024,
		0.0015773496022817167, 0.0016413334084849302, 0.0017068354011726512,
		0.0017738657468903503, 0.0018424107865905371, 0.0019124473527039961,
		0.0019839905437757511, 0.0020570044913864856, 0.0021314772554310206,
		0.0022073935243709774, 0.0022847435564257133, 0.0023634945231977844,
		0.0024436264036744332, 0.0025251133220893333, 0.0026079357701856605,
		0.0026920587401893263, 0.0027774521193260461, 0.0028640791633547311,
		0.0029519081765462122, 0.0030409006777833555, 0.0031310205981640729,
		0.0032222250469656809, 0.0033144685281284786, 0.0034077089852878432,
		0.0035018951402814484, 0.003596988201224169, 0.0036929278229154137,
		0.0037896678870492553, 0.0038871566241322287, 0.0039853261730970861,
		0.0040841294169390663, 0.0041835059838843079, 0.0042833933387723512,
		0.0043837261368993262, 0.0044844488974304479, 0.0045854932957565195,
		0.0046867921209211068, 0.0047882735931617116, 0.0048898745755680383,
		0.0049915231788431611, 0.0050931497285813497, 0.0051946791297664465,
		0.0052960417422814373, 0.0053971612955007083, 0.0054979658306310002,
		0.0055983797994516738, 0.0056983253408818286, 0.0057977277900710658,
		0.0058965061241457476, 0.0059945906110298215, 0.0060918990398812472,
		0.0061883544421090396, 0.0062838835100025494, 0.0063784040723008813,
		0.0064718392683248841, 0.0065641156880257012, 0.0066551575218083692,
		0.0067448837961064734, 0.0068332211341646538, 0.0069200954455324835,
		0.0070054334691378312, 0.0070891574289693657, 0.0071711976967911263,
		0.0072514833710739506, 0.0073299462238696437, 0.0074065136345697734,
		0.0074811207162423544, 0.0075536986382993528, 0.00762418337802596,
		0.0076925122443991002, 0.0077586234882858086, 0.0078224582372105698,
		0.0078839551310774785, 0.0079430619696299314, 0.0079997234806384564,
		0.0080538863175640511, 0.0081055017546585971, 0.0081545228891952252,
		0.0082009008815607833, 0.008244594453582119, 0.0082855652960434623,
		0.0083237733919798405, 0.0083591810162194566, 0.0083917558821141533,
		0.0084214702626896412, 0.0084482939561720712, 0.0084722007770392724,
		0.0084931682078880754, 0.0085111792876931842, 0.0085262133908074918,
		0.0085382578791410729, 0.0085473009945351702, 0.0085533341831194563,
		0.0085563512135527313, 0.0085563512135527313, 0.0085533341831194563,
		0.0085473009945351702, 0.0085382578791410729, 0.0085262133908074918,
		0.0085111792876931842, 0.0084931682078880754, 0.0084722007770392724,
		0.0084482939561720712, 0.0084214702626896412, 0.0083917558821141533,
		0.0083591810162194566, 0.0083237733919798405, 0.0082855652960434623,
		0.008244594453582119, 0.0082009008815607833, 0.0081545228891952252,
		0.0081055017546585971, 0.0080538863175640511, 0.0079997234806384564,
		0.0079430619696299314, 0.0078839551310774785, 0.0078224582372105698,
		0.0077586234882858086, 0.0076925122443991002, 0.00762418337802596,
		0.0075536986382993528, 0.0074811207162423544, 0.0074065136345697734,
		0.0073299462238696437, 0.0072514833710739506, 0.0071711976967911263,
		0.0070891574289693657, 0.0070054334691378312, 0.0069200954455324835,
		0.0068332211341646538, 0.0067448837961064734, 0.0066551575218083692,
		0.0065641156880257012, 0.0064718392683248841, 0.0063784040723008813,
		0.0062838835100025494, 0.0061883544421090396, 0.0060918990398812472,
		0.0059945906110298215, 0.0058965061241457476, 0.0057977277900710658,
		0.0056983253408818286, 0.0055983797994516738, 0.0054979658306310002,
		0.0053971612955007083, 0.0052960417422814373, 0.0051946791297664465,
		0.0050931497285813497, 0.0049915231788431611, 0.0048898745755680383,
		0.0047882735931617116, 0.0046867921209211068, 0.0045854932957565195,
		0.0044844488974304479, 0.0043837261368993262, 0.0042833933387723512,
		0.0041835059838843079, 0.0040841294169390663, 0.0039853261730970861,
		0.0038871566241322287, 0.0037896678870492553, 0.0036929278229154137,
		0.003596988201224169, 0.0035018951402814484, 0.0034077089852878432,
		0.0033144685281284786, 0.0032222250469656809, 0.0031310205981640729,
		0.0030409006777833555, 0.0029519081765462122, 0.0028640791633547311,
		0.0027774521193260461, 0.0026920587401893263, 0.0026079357701856605,
		0.0025251133220893333, 0.0024436264036744332, 0.0023634945231977844,
		0.0022847435564257133, 0.0022073935243709774, 0.0021314772554310206,
		0.0020570044913864856, 0.0019839905437757511, 0.0019124473527039961,
		0.0018424107865905371, 0.0017738657468903503, 0.0017068354011726512,
		0.0016413334084849302, 0.0015773496022817167, 0.001514901520821024,
		0.0014539800651395143, 0.0013945919856421184, 0.0013367332506084143,
		0.0012804046232005564, 0.0012256044570541449, 0.0011723248197248871,
		0.0011205575804490652, 0.0010702884133779744, 0.0010215107156041454,
		0.00097421127156387751, 0.00092838867936580823, 0.00088402254995322081,
		0.0008410975840678226, 0.00079958110240744553, 0.00075947631501319265,
		0.00072076580050502625, 0.00068343468905098371, 0.0006474127548303588,
		0.00061275090975397421, 0.00057938190608542079, 0.0005472828402515276,
		0.00051645354569522907, 0.00048684430822789812, 0.0004584503987046613,
		0.0004312340532734016, 0.00040518081307119023, 0.00038025899861406803,
		0.00035644549456481851, 0.00033370999234300623, 0.00031202217045904067,
		0.00029134995579086707, 0.00027165870771194241, 0.00025292651954488888,
		0.00023512578116460171, 0.0002182498128765064, 0.00020226733820720105,
		0.00018715745983810908, 0.00017284015891234515, 0.00015929463718936134,
		0.00014650139507745382, 0.00013459636946903071, 0.0001232480108380004,
		0.00011265121488255643, 0.00010273336557745961, 0.000093420483263056495,
		0.000084749185850655908, 0.000076645878781258207,
		0.000069128401103763559, 0.000062140388562404174,
		0.000055692601758028553, 0.000049739932687524062,
		0.000044291245517772101, 0.000039309720691864923,
		0.000034808770994987535, 0.00003076222082586599,
		0.000027190109935793652, 0.0000240808761912665, 0.000021470175773501382,
		0.00001936753284143648, 0.000017833760715256158,
		0.000016914664698078393, 0.000016714489931227413,
		0.000017335114024242748, -0.000074029787723175851 };

