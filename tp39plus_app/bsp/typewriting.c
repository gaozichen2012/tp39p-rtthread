//ÊäÈë·¨Çı¶¯£¬À¬»øÇı¶¯£¬Ö±½Ó´ÓÀÏ39PÒÆÖ²£¬ÎŞĞèÁË½âÀ¬»ø

#define TYPEWRITING
#include "typewriting.h"
#include "String.h"
#include "lcd_api.h"
#include "language.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "font.h"
#include "main_process.h"
#include "telephone.h"
#include "tom_interface.h"
/*Æ´ÒôÊäÈë·¨²éÑ¯Âë±í*/
typedef struct
{
    const char *T9;
    const char *PY;
    const unsigned char *MB;
} T9PY_IDX;

T9PY_IDX *t9[20];

const char KeyPinyinArray[9][8] =
    {
        {',', '.', '?', ',', '.', '?', '\0', '\0'},
        {'a', 'b', 'c', 'A', 'B', 'C', '\0', '\0'},
        {'d', 'e', 'f', 'D', 'E', 'F', '\0', '\0'},
        {'g', 'h', 'i', 'G', 'H', 'i', '\0', '\0'},
        {'j', 'k', 'l', 'J', 'K', 'L', '\0', '\0'},
        {'m', 'n', 'o', 'M', 'N', 'O', '\0', '\0'},
        {'p', 'q', 'r', 's', 'P', 'Q', 'R', 'S'},
        {'t', 'u', 'v', 'T', 'U', 'V', '\0', '\0'},
        {'w', 'x', 'y', 'z', 'W', 'X', 'Y', 'Z'},
};

const char SymbleTable[] =
    {
        ' ',  /*space*/
        '!',  /**/
        '\"', /**/
        '#',  /**/
        '$',  /**/
        '%',  /**/
        '&',  /**/
        ',',  /**/
        '(',  /**/
        ')',  /**/
        '*',  /**/
        '+',  /**/
        '-',  /**/
        '=',  /**/
        ':',  /**/
        ';',  /**/
        '@',  /**/
        ',',  /**/
        '.',  /**/
        '/',  /**/
        '`',  /**/
        '\\', /**/
        '[',  /**/
        ']',  /**/
        '{',  /**/
        '}',  /**/
        '<',  /**/
        '>',  /**/
        '^',  /**/
        '?',  /**/
        '_',  /**/
};

//"Æ´ÒôÊäÈë·¨ºº×ÖÅÅÁĞ±í"
const unsigned char PY_mb_space[] = {""}; //À´ ×Ô £ºST_M8._C N
const unsigned char PY_mb_a[] = {"°¡°¢ß¹àÄëçï¹ºÇ°²°´°®°µ°£"};
const unsigned char PY_mb_ai[] = {"°®°£°­°«°¤°¦°¥°§°¨°©°ª°¬°¯´ôŞßàÈàÉæÈè¨êÓíÁïÍö°"};
const unsigned char PY_mb_an[] = {"°²°´°µ°¶°¸°³°±°·°°ÚÏÛûŞîáíâÖèñï§ğÆ÷ö¹ã³§"};
const unsigned char PY_mb_ang[] = {"°º°¹°»"};
const unsigned char PY_mb_ao[] = {"°¼°Â°½°¾°¿°À°Á°Ã°ÄÏùÛêŞÖà»á®âÚåÛæÁæñéáñúòüöË÷¡÷é"};
const unsigned char PY_mb_b[] = {"²»²¿±¾±«°Ø±Ï±ä±È²¢±ğ°Ù±¨²½°ì±í"};
const unsigned char PY_mb_ba[] = {"°Ñ°Ë°É°Í°Î°Ô°Õ°Ö°Ó°Å°Æ°Ç°È°Ê°Ì°Ï°Ğ°ÒÜØİÃá±å±îÙôÎöÑ÷É"};
const unsigned char PY_mb_bai[] = {"°Ù°×°Ü°Ú²®°Ø°Û°İ°ŞŞãêş"};
const unsigned char PY_mb_ban[] = {"°ì°ë°å°à°ã°æ°è°á°ß°â°é°ä°ç°ê°íÚæÛàîÓñ£ñ­ô²"};
const unsigned char PY_mb_bang[] = {"°ï°ô°î°ñ°ğ°ò°ó°õ°ö°÷°ø°ùİòäº"};
const unsigned char PY_mb_bao[] = {"±¨±£°ü°ş±¡°û±©±¦±¥±§±¬ÅÚ±¤ÅÙ°ú°ı±¢±ª±«ÆØÆÙİáæßìÒğ±ñÙõÀöµÙèå²"};
const unsigned char PY_mb_bei[] = {"±±±»±¶±¸±³±²±´±­±°±Û±¯±®±µ±·±¹±ºØÃÚéÚıÛıİÉİíßÂã£íÕğÇñØöÍ÷¹"};
const unsigned char PY_mb_ben[] = {"±¾±¼±½±¿º»ÛÎÛĞêÚï¼"};
const unsigned char PY_mb_beng[] = {"±Ã±À°ö±Á±Â±Ä±ÅàÔê´"};
const unsigned char PY_mb_bi[] = {"±È±Ø±Ü±Õ±Ù±Ê±Ú±Û±Ï±Ë±Æ±Ò±Ç±ÎÃØ±É±Ì±Í±Ğ±Ñ±Ó±Ô±Ö±×±İ·÷ÃÚØ°ÙÂÛıÜÅÜêİ©İÉŞµßÁßÙáùâØã¹ääå¨åöåşæ¾æÔèµêÚî¢î¯îéïõñÔóÙóëó÷ô°ôÅõÏ÷Â"};
const unsigned char PY_mb_bian[] = {"±ä±ß±ã±à±é±ç±â±æ±Ş±á±å±èØÒÛÍÜĞâíãêçÂì®ìÔí¾íÜñ¹ñÛòùóÖöı±Ë°¶"}; //À´ ×Ô £ºST_M8._C N
const unsigned char PY_mb_biao[] = {"±í±ê±ë±ìÉ×æ»æôè¼ì©ì­ïÚïğñ¦ñÑ÷§÷Ô"};
const unsigned char PY_mb_bie[] = {"±ğ±î±ï±ñõ¿"};
const unsigned char PY_mb_bin[] = {"±ö±ò±ó±ô±õ±÷ÙÏáÙçÍçãéÄéëë÷ïÙ÷Æ÷Ş"};
const unsigned char PY_mb_bing[] = {"²¢²¡±ø±ú±ù±û±ı±ü±şÆÁÙ÷ÚûŞğéÄ"};
const unsigned char PY_mb_bo[] = {"²¨²¥°ş²®²¦±¡²©²ª²µ²£²´°Ø²·²¤²§²«²¬²­²¯²°²±²²²³ÆÇØÃÙñà£âÄéŞêşë¢íçîàğ¾ô¤õÀõËõÛ"};
const unsigned char PY_mb_bu[] = {"²»²¿²½²¼²¹²¶ÆÒ²·±¤²¸²º²¾²Àß²åÍê³êÎîĞîßõ³"};
const unsigned char PY_mb_c[] = {"²ú³É³Ì³Â²Ì²Ü´Ş³şñÒ³Ø³ğêËá¯´Ó´Ë´¦³µ³¡´«³§²Å´Î²é"};
const unsigned char PY_mb_ca[] = {"²Á²ğàêíå"};
const unsigned char PY_mb_cai[] = {"²É²Å²Ä²Ë²Æ²Ã²Ê²Â²Ç²È²Ì"};
const unsigned char PY_mb_can[] = {"²Î²Ğ²Ï²Ó²Í²Ñ²Òåîæîè²ôÓ÷õ"};
const unsigned char PY_mb_cang[] = {"²Ø²Ö²Ô²Õ²×"};
const unsigned char PY_mb_cao[] = {"²İ²Û²Ù²Ú²ÜàĞäîó©ô½"};
const unsigned char PY_mb_ce[] = {"²â²ß²à²á²Şâü"};
const unsigned char PY_mb_cen[] = {"²Îá¯ä¹"};
const unsigned char PY_mb_ceng[] = {"²ãÔø²äàá"};
const unsigned char PY_mb_ch[] = {"²î³¤³£²ı³¬³Â³Ì³Ç³ÖÖØ³ö³õ"};
const unsigned char PY_mb_cha[] = {"²é²î²å²ì²è²æ²ç²ê²ë²í²ïÉ²ÔûàêâªâÇãâæ±è¾é«é¶éßïÊïïñÃ"};
const unsigned char PY_mb_chai[] = {"²î²ñ²ğ²òÙ­îÎğûò²"};
const unsigned char PY_mb_chan[] = {"²úµ¥²ù²û²ó²ô²õ²ö²÷²ø²üÙæÚÆİÛâÜâãäıå¤åîåñæ¿æöêèìøïâó¸õğ"};
const unsigned char PY_mb_chang[] = {"³¤³£³¡³§³ª³¦²ı³«³¥³©²ş³¢³¨ÉÑÌÈØöÛËÜÉİÅáäâêã®ãÑæ½æÏêÆë©öğ"};
const unsigned char PY_mb_chao[] = {"³¯³¬³±³²³­³®³°³³³´´Â½Ëâ÷êËìÌñé"};
const unsigned char PY_mb_che[] = {"³µ³¹³ß³·³¶³¸³ºÛåíº"};
const unsigned char PY_mb_chen[] = {"³Â³Á³Æ³Ä³¾³¼³¿Éò³»³½³À³ÃØ÷ÚÈÚßŞÓàÁå·è¡é´í×ö³"};
const unsigned char PY_mb_cheng[] = {"³É³Ì³Æ³Ç³Ğ³Ë³ÊÊ¢³Å³Ï³È³Í³Î³Ñ³Ò³ÓØ©ÛôàáèÇèßëóîªîñîõñÎòÉõ¨"};
const unsigned char PY_mb_chi[] = {"³Ö³ß³İ³Ô³à³Ø³Ù³á³â³Ü³Õ³×³Ú³Û³Ş³ãÙÑÛæÜ¯Üİß³ßêà´àÍáÜâÁæÊë·í÷ğ·ñ¡ñİò¿ó¤ó×óøôùõØ÷Î"};
const unsigned char PY_mb_chong[] = {"ÖÖ³æÖØ³ä³å³çÓ¿³èÜûâçã¿ï¥ô©ô¾"};
const unsigned char PY_mb_chou[] = {"³é³ğ³ô³ê³ë³ì³í³î³ï³ñ³ò³óÙ±àüã°ñ¬öÅ"};
const unsigned char PY_mb_chu[] = {"³ö´¦³ı³õ´¡´¥³ş³ú´¢Ğó³÷³ø³ù³û³ü´£´¤Ø¡Û»âğãÀç©èÆèúéËñÒòÜõé÷íåø"};
const unsigned char PY_mb_chuai[] = {"´§Şõà¨àÜëúõß"};
const unsigned char PY_mb_chuan[] = {"´«´¬´©´®´¨´ª´­â¶å×çİë°îËô­"};
const unsigned char PY_mb_chuang[] = {"´²´´´°´³´¯´±âë"};
const unsigned char PY_mb_chui[] = {"´µ´¹´¸×µ´¶´·Úïé¢é³"};
const unsigned char PY_mb_chun[] = {"´º´¿´¼´»´½´¾´Àİ»ğÈòí"};
const unsigned char PY_mb_chuo[] = {"´Á´Âà¨ê¡õÖöº"};
const unsigned char PY_mb_ci[] = {"´Ë´Î´Ì´Å´Æ´Ê´Ä´Ã´Ç´È´É´ÍËÅ×ÈÜëßÚìôğËôÙ"};
const unsigned char PY_mb_cong[] = {"´Ó´Ô´Ï´Ğ´Ñ´ÒÜÊäÈæõçıè®"};
const unsigned char PY_mb_cou[] = {"´Õé¨ê£ëí"};
const unsigned char PY_mb_cu[] = {"´Ö´Ù´×´Ø×äİıáŞâ§éãõ¡õ¾õí"};
const unsigned char PY_mb_cuan[] = {"´Û´Ú´ÜÔÜÙàß¥ìàïé"};
const unsigned char PY_mb_cui[] = {"´ß´à´ã´â´İË¥´Ş´á´äİÍßıã²è­éÁë¥ö¿"}; //À´ ×Ô £ºST_M8._C N
const unsigned char PY_mb_cun[] = {"´æ´å´ç¶×ââñå"};
const unsigned char PY_mb_cuo[] = {"´í´ë´é´è´ê´ìØÈáÏëâï±ïóğîğûõºõã"};
const unsigned char PY_mb_d[] = {"µÄ´óµØ¶Å¶­¶Î´÷µË¶¡ñ¼µÒµÔµóµçµ³¶¨µÈµ±¶Óµã´úµÀ"};
const unsigned char PY_mb_da[] = {"´ó´ò´ï´ğ´îËş´ñŞÇßÕàªâòæ§í³ğãñ×óÎ÷°÷²"};
const unsigned char PY_mb_dai[] = {"´ó´ú´ø´ı´ü´÷´ô´õ´ö´ù´û´şµ¡Ü¤ß°ß¾á·åÊææçªçé÷ì"};
const unsigned char PY_mb_dan[] = {"µ¥µ«µ¯µ£Ê¯µ°µ­µ¨µªµ¤µ©µ¢µ¦µ§µ¬µ®ÙÙİÌà¢å£ééêæíñğãğ÷ñõóì"};
const unsigned char PY_mb_dang[] = {"µ³µ±µµµ²µ´ÚÔÛÊİĞå´í¸ñÉ"};
const unsigned char PY_mb_dao[] = {"µ½µÀµ¼µ¶µ¹µ¾µºµ·µÁµ¸µ»µ¿ß¶àüâáë®ìâôî"};
const unsigned char PY_mb_de[] = {"µÄµÃµØµÂµ×ï½"};
const unsigned char PY_mb_deng[] = {"µÈµÆµÇµË³ÎµÅµÉµÊàâáØê­íãïëô£"};
const unsigned char PY_mb_di[] = {"µÄµØµÚµÍµĞµ×ÌáµÛµÖµÎµÜµİµÌµÏµÑµÒµÓµÔµÕµÙµŞØµÙáÚ®ÚĞÛ¡Ûæİ¶àÖæ·ç°èÜé¦êëíÆíÚíûïáôÆ÷¾"};
const unsigned char PY_mb_dia[] = {"àÇ"};
const unsigned char PY_mb_dian[] = {"µçµãµæµäµêµßµíµàµáµâµåµèµéµëµìµîÚçÛãáÛçèîäñ°ñ²ô¡õÚ"};
const unsigned char PY_mb_diao[] = {"µ÷µôµõÄñµïµğµñµòµóµöîöï¢õõöô"};
const unsigned char PY_mb_die[] = {"µüµøµùµúµûµıµşÛìÜ¦Şéà©ëºğ¬ñóõÚõŞöø"};
const unsigned char PY_mb_ding[] = {"¶¨¶¥¶¤¶¡¶©¶¢¶£¶¦¶§Øêà¤çàëëíÖî®îúğÛñôôú"};
const unsigned char PY_mb_diu[] = {"¶ªîû"};
const unsigned char PY_mb_dong[] = {"¶¯¶«¶¬¶®¶´¶³¶­¶°¶±¶²ÛíßËá´á¼ë±ëËëØíÏğ´"};
const unsigned char PY_mb_dou[] = {"¶·¶¼¶¹¶Á¶µ¶¶¶¸¶º¶»İúñ¼ò½óû"};
const unsigned char PY_mb_du[] = {"¶È¶¼¶¾¶À¶Á¶É¶Å¶Â¶Æ¶Ù¶½¶¿¶Ã¶Ä¶Ç¶ÊÜ¶à½äÂèüë¹ó¼óÆ÷Ç÷ò"};
const unsigned char PY_mb_duan[] = {"¶Ï¶Ë¶Î¶Ì¶Í¶Ğé²ìÑóı"};
const unsigned char PY_mb_dui[] = {"¶Ô¶Ó¶Ñ¶Ø¶Òí¡í­íÔ"};
const unsigned char PY_mb_dun[] = {"¶Ü¶Ö¶Ù¶×¶Ø¶Õ¶Ú¶Û¶İãçìÀí»íâíïïæõ»"};
const unsigned char PY_mb_duo[] = {"¶à¶È¶á¶ä¶Ş¶ß¶â¶ã¶å¶æ¶ç¶è¶éÍÔßÍßáãõç¶èŞîìñÖõâ"};
const unsigned char PY_mb_e[] = {"¶ñ¶î¶÷¶í¶ú¶ü¶ê¶ö¶ë¶ì¶ï¶ğ¶ò¶ó¶ô¶õØ¬ÚÌÛÑÜÃİ­İàßÀãµåíæ¹éîëñï°ïÉğÊò¦öù"};
const unsigned char PY_mb_ei[] = {"ÚÀ"};
const unsigned char PY_mb_en[] = {"¶÷İìŞô"};
const unsigned char PY_mb_er[] = {"¶ø¶ş¶û¶ù¶ú¶ü¶ı·¡Ù¦åÇçíîïğ¹öÜ"};
const unsigned char PY_mb_f[] = {"·Ö·¢·¶·®·½·ë·´·Å¸´·ç·À·Ç·â¸»·­"};
const unsigned char PY_mb_fa[] = {"·¢·¨·§·¦·¥·£·¤·©ÛÒíÀ"};
const unsigned char PY_mb_fan[] = {"·´·­·¶·¸·¹·±·º·¬·²·³·µ·ª·«·®·¯·°··Ş¬ŞÀá¦èóìÜî²õì"};
const unsigned char PY_mb_fang[] = {"·½·Å·À·Ã·¿·Ä·Â·Á·¼·¾·»ÚúáİèÊîÕô³öĞ"};
const unsigned char PY_mb_fei[] = {"·Ç·Ê·É·Ñ·Ï·Î·Ğ·Æ·Ë·È·Ì·ÍÜÀáôã­äÇåúç³é¼ëèì³ìéïĞğòòãóõôäö­öî"};
const unsigned char PY_mb_fen[] = {"·Ö·Û·Ü·İ·à·×·Ò·ß·Ó·Ô·Õ·Ø·Ù·Ú·ŞÙÇå¯çãèûö÷÷÷"};
const unsigned char PY_mb_feng[] = {"·ç·â·ä·á·ì·å·æ·è·î·ã·é·ê·ë·í·ïÙºÛºİ×ßôããí¿"};
const unsigned char PY_mb_fo[] = {"·ğ"};
const unsigned char PY_mb_fou[] = {"·ñó¾"};
const unsigned char PY_mb_fu[] = {"¸´·ş¸±¸®·ò¸º¸»¸½¸£·ü·û·ù¸¯¸¡¸¨¸¶¸¹¸¾·õ¸²·ö·ø¸µ·ğ¸¿¸¸¸¥¸¦·ô·ú·ó·÷·ı¸¢¸¤¸§¸©¸ª¸«¸¬¸­¸°¸³¸·¸¼¸ÀÙëÙìÛ®Ü½ÜÀÜŞÜòİ³İÊŞÔß»á¥âöäæåõæÚæâç¦ç¨èõêçìğíÉíêíëî·ïûğ¥ò¶òİòğòóôïõÃõÆöÖöû"};
const unsigned char PY_mb_g[] = {"¸ö¹ú¹¤¹ı¸ß¹ù¸ê¸ğ¹¢¹®¹¨¸Ê¸É¹Å¼Ö¹È¹Ë¹Ø¹Ü¸÷¸ï¹«¸ù¸ü¸Ä¹â¹ã¹û¸ñ¹²¸ã"};
const unsigned char PY_mb_ga[] = {"¼Ğ¸Á¸Â¸ìŞÎæØæÙê¸îÅ"}; //À´ ×Ô £ºST_M8._C N
const unsigned char PY_mb_gai[] = {"¸Ä¸Ã¸Ç¸Å¸Æ¸È½æØ¤ÚëÛòê®êà"};
const unsigned char PY_mb_gan[] = {"¸É¸Ë¸Ğ¸Ò¸Ï¸Ê¸Î¸Ñ¸Ì¸Í¸ÓÇ¬ÛáÜÕŞÏß¦ãïäÆä÷ç¤éÏêºí·ğáôû"};
const unsigned char PY_mb_gang[] = {"¸Õ¸Ö¸×¸Ù¸Ú¸Û¸Ü¸Ô¸Ø¿¸í°î¸óà"};
const unsigned char PY_mb_gao[] = {"¸ß¸ã¸æ¸å¸à¸İ¸Ş¸á¸â¸äØºÚ¾Û¬Ş»çÉéÀéÂê½ï¯"};
const unsigned char PY_mb_ge[] = {"¸ö¸÷¸ïºÏ¸ñ¸î¸è¸ô¸Ç¸ç¸ğ¸õ¸ó¸ê¸é¸ë¸ì¸í¸ò¿©ÒÙØªØîÛÁÛÙÜªàÃæüë¡ëõíÑïÓñËò¢ò´ô´÷À"};
const unsigned char PY_mb_gei[] = {"¸ø"};
const unsigned char PY_mb_gen[] = {"¸ù¸úØ¨İ¢ßçôŞ"};
const unsigned char PY_mb_geng[] = {"¸ü¸û¾±¸ı¸ş¹¡¹¢¹£ßìâÙç®öá"};
const unsigned char PY_mb_gong[] = {"¹¤¹«¹²¹©¹¦¹¥ºì¹®¹±¹¯¹¬¹§¹¨¹ª¹­¹°çîëÅò¼ö¡"};
const unsigned char PY_mb_gou[] = {"¹»¹¹¹µ¾ä¹·¹³¹´¹º¹¶¹¸ØşÚ¸á¸åÜæÅçÃèÛêíì°óÑóô÷¸"};
const unsigned char PY_mb_gu[] = {"¹Ä¹Ì¹Å¹Ç¹Ê¹Ë¹É¹È¹À¹Í¹Â¹Ã¼Ö¹¼¹½¹¾¹¿¹Á¹ÆØÅÚ¬İÔßÉáÄãéèôéïêôêöëûì±î­î¹îÜïÀğ³ğÀğóòÁôşõıöñ÷½"};
const unsigned char PY_mb_gua[] = {"¹Ò¹Î¹ÏÀ¨¹Ğ¹Ñ¹ÓØÔÚ´ßÉèéëÒğ»"};
const unsigned char PY_mb_guai[] = {"¹Ö¹Ô¹ÕŞâ"};
const unsigned char PY_mb_guan[] = {"¹Ø¹Ü¹Û¹Ù¹à¹á¹ß¹Ú¹İ¹Ş¹×ÂÚÙÄİ¸ŞèäÊîÂğÙñæ÷¤"};
const unsigned char PY_mb_guang[] = {"¹â¹ã¹äßÛáîèæë×"};
const unsigned char PY_mb_gui[] = {"¹æ¹ó¹é¹è¹í¹ì¹ê¹ğ¹å¹ç¹ë¹î¹ï¹ñ¹ò¹ô¿şÈ²ØĞØÛâÑå³æ£èíêÁêĞğ§óşöÙ÷¬"};
const unsigned char PY_mb_gun[] = {"¹ö¹õ¹÷ÙòçµíŞöç"};
const unsigned char PY_mb_guo[] = {"¹ú¹ı¹û¹ø¹ùÎĞ¹üÙåÛöŞâßÃàşáÆâ£é¤ë½ñøòäòå"};
const unsigned char PY_mb_h[] = {"ºÍ»áºóºÎ»ÆºúºéºîºØ»ôìèº«»ªºÂ»¯ºÃºÏ»¹»îºÜ»ØºÅºÁ»¨»òº£»°ºì»ğº¦"};
const unsigned char PY_mb_ha[] = {"¹ş¸òÏºîş"};
const unsigned char PY_mb_hai[] = {"»¹º£º¦º¢º¡º¤º¥º§ºÙ¿ÈàËëÜõ°"};
const unsigned char PY_mb_han[] = {"º¬º¸ºµº°ººº®º¹º¯º«º¨º©ºªº­º±º²º³º´º¶º·ÚõİÕŞşãÛå«êÏìÊñüò¥òÀ÷ı"};
const unsigned char PY_mb_hang[] = {"ĞĞº½º»º¼¿ÔÏïãìç¬çññş"};
const unsigned char PY_mb_hao[] = {"ºÃºÅºÁºÄºÀºÂºÆ¸äº¾º¿ºÑİïŞ¶àÆàãå©å°ê»ğ©ò«òº"};
const unsigned char PY_mb_he[] = {"ºÍºÏºÓºÎºËºÕºÉºÖºÈºØºÇºÌÏÅºĞºÊºÑºÒºÔº×Ú­ÛÀÛÖàÀãØæüêÂîÁò¢òÂôç"};
const unsigned char PY_mb_hei[] = {"ºÚºÙàË"};
const unsigned char PY_mb_hen[] = {"ºÜºİºÛºŞ"};
const unsigned char PY_mb_heng[] = {"ºáºâºãºßºàŞ¿ĞĞçñèì"};
const unsigned char PY_mb_hong[] = {"ºìºéºäºæºåºçºèºêºëÙäÙêÚ§İ¦Ş®Ş°ãÈãü"};
const unsigned char PY_mb_hou[] = {"ºóºòºñºîºíºïºğÜ©ááåËğúóóô×ö×÷¿"};
const unsigned char PY_mb_hu[] = {"ºÍ»¤»¥ºşºô»§»¡ºõºúºı»¢ºöºËº÷Ï·ºøºùºûºü»£»¦Ùüßüàñá²â©âïã±ä°äïçúéÎéõì²ìÃìÎìæìèìïğ­ğÀğÉğ×óËõ­õú"};
const unsigned char PY_mb_hua[] = {"»¯»¨»°»®»¬»ª»­»©»«»íæèèëí¹îü"};
const unsigned char PY_mb_huai[] = {"»µ»³»®»´»±»²õ×"};
const unsigned char PY_mb_huan[] = {"»¹»·»»»¶»º»¼»Ã»À»¸»½»¾»¿»Á»ÂÛ¨Û¼İÈß§à÷âµä¡ä½äñå¾åÕçÙïÌöé÷ß"};
const unsigned char PY_mb_huang[] = {"»Æ»É»Ä»Ê»Å»È»Ç»Ë»Ì»Í»Î»Ï»Ğ»ÑÚòáåäÒäêåØè«ëÁñ¥ó¨óòöü"};
const unsigned char PY_mb_hui[] = {"»á»Ø»Ò»Ó»Ô»ã»Ù»Û»Ö»æ»İ»ÕÀ£¶é»²»×»Ú»Ü»Ş»ß»à»â»ä»åÚ¶ÜîÜöŞ¥ßÔßÜà¹ãÄä§ä«åççÀçõèíêÍí£ò³ó³÷â"};
const unsigned char PY_mb_hun[] = {"»ì»ë»ç»è»é»êÚ»âÆãÔäã"};
const unsigned char PY_mb_huo[] = {"ºÍ»î»ò»ğ»õ»ñ»ï»ô»í»ó»öØåŞ½ß«àëâ·îØïÁïìñëó¶"};
const unsigned char PY_mb_j[] = {"¼¶¼Ö½­½¯½ª½¹½ğ½ù½ú¾Í½ø¼Ó¾­¼Ò½á½×¼ä¼ş¼°½â½Ï»ú¾ü½Ó½Ì¼«¾Å¼û"};
const unsigned char PY_mb_ji[] = {"¼¶¼°»ú¼«¼¸»ı¸ø»ùÆä¼Ç¼ºÆÚÏµ¸ï¼Æ¼¯¼´¼Ê¼¾¼¤¼Ã¼¼»÷¼Ì¼±¼Á¼È¼Í¼Ä¼·¼¦¼£¼¨ÆëÆæ¼ª¼¹¼­¼®¼²¼¡¼¬»û»ø»ü»ş¼¢¼¥¼§¼©¼³¼µ¼»¼½¼¿¼À¼Â¼Å¼É¼Ë½åØ¢Ø½ØÀØŞÙ¥ÙÊÚµÛÔÜ¸ÜÁÜùİğŞªŞáß´ßÒßâßóá§áÕä©åìæ÷çÜçáé®éêêªê«êåêéê÷ì´í¶î¿ïúğ¢ñ¤ò±óÅóÇôßõÒõÕö«öİöê÷Ù÷ä"};
const unsigned char PY_mb_jia[] = {"¼Ó¼Ò¼Ü¼Û¼×¼Ğ¼Ù¼Ø¼Ö¼Ú¼İ¼Î¼Ï¼Ñ¼Ô¼Õ¼ŞÇÑĞ®ØÅÙ¤Û£İçáµä¤åÈçìê©ëÎí¢îòïØğèğıñÊòÌóÕôÂõÊ"};
const unsigned char PY_mb_jian[] = {"¼ä¼ş¼û½¨¼á¼õ¼ì¼ù¼â¼ò¼î¼ô¼è½¥¼ç¼ü½¡¼í¼ø½£¼ß¼à¼æÇ³¼é¼ı¼ë½¢¼ó¼ã¼å¼ê¼ï¼ğ¼ñ¼ö¼÷¼ú½¤½¦½§ÙÔÚÉÚÙİÑİóŞöàîäÕå¿åÀçÌèÅé¥ê§ê¯êğêùë¦ëìíúïµğÏñĞóÈôåõÂõİöä÷µ"};
const unsigned char PY_mb_jiang[] = {"½«½µ½²Ç¿½­½¬½¯½±½®½©½ª½°½³½´Üüä®ç­çÖêñíäñğôİôø"};
const unsigned char PY_mb_jiao[] = {"½Ï½Ì½»½Ç½Ğ½Å½º½½½¹¾õĞ£½¶½Á½Í½¼½Â½Ñ½·½¸½¾½¿½À½Ã½Ä½Æ½È½É½Ê½Ë½ÎÙ®ÙÕÜ´ÜúŞØàİá½áèæ¯ë¸ğ¨ğÔòÔõ´õÓöŞ"};
const unsigned char PY_mb_jie[] = {"½á½×½â½Ó½Ú¼Ò½ç½Ø½é½è¼Û½ì½Ö½Ò½à½Ü½ß½Ô½Õ½Ù½Û½İ½Ş½ã½ä½å½æ½ê½ë¿¬ÙÊÚ¦ÚµŞ×à®àµæ¼æİèîíÙğÜò¡ò»ôÉöÚ÷º"};
const unsigned char PY_mb_jin[] = {"½ø½ğ½ü½ô½ï½ñ¾¡½ö¾¢½ş½û½ò½î½õ½ú½í½ó½÷½ù½ıÚáİ£İÀàäâËâÛæ¡çÆèªéÈêáêîñÆñæ"};
const unsigned char PY_mb_jing[] = {"¾­¾«¾©¾¶¾®¾²¾¹¾§¾»¾³¾µ¾°¾¯¾¢¾¥¾´¾ª¾¦¾º¾£¾¤¾¨¾¬¾·¾¸ØÙÙÓÚåİ¼â°ã½ãşåÉåòæºëÂëÖëæìºö¦"};
const unsigned char PY_mb_jiong[] = {"¾¼¾½åÄêÁìç"};
const unsigned char PY_mb_jiu[] = {"¾Í¾Å¾É¾¿¾Ã¾È¾Æ¾À¾¾¾Á¾Â¾Ä¾Ç¾Ê¾Ë¾Ì¾ÎÙÖà±ãÎèÑèêğ¯ğÕôñ÷İ"};
const unsigned char PY_mb_ju[] = {"¾ß¾İ¾Ö¾Ù³µ¾ä¾Û¾à¾Ş¾Ó¾â¾ç¾Ø¾Ü¹ñ½Û¾Ï¾Ğ¾Ñ¾Ò¾Ô¾Õ¾×¾Ú¾á¾ã¾å¾æÙÆÚªÜÄÜÚÜìŞäåáåğè¢èÛé§é°é·éÙêøì«îÒï¸ñÀñÕôòõ¶õáö´öÂöÄ÷¶"};
const unsigned char PY_mb_juan[] = {"¾í¾è¾é¾ê¾ë¾ì¾î¿¡Û²áúä¸èğîÃïÃïÔöÁ"};
const unsigned char PY_mb_jue[] = {"¾ö¾õ¾ø½Ç½Å¾ò½À¾ï¾ğ¾ñ¾ó¾ô¾÷ØÊØãÚÜÛÇŞ§àµàÙàåáÈâ±æŞçåèöéÓìßïãõêõû¾Ş¶î"};
const unsigned char PY_mb_jun[] = {"¾ü¾ù¾ú¾ı¹ê¾û¾ş¿¡¿¢¿£¿¤¿¥ŞÜñäóŞ÷å"};
const unsigned char PY_mb_k[] = {"¿É¿ª¿´Øá¿Ë¿×Ú÷¿Ü¿Æ¿Ú¿Õ¿à¿ì¿¼¿Ì¿¿¿ö¿éÀ§¿í¿ó¿¹¿¨"};
const unsigned char PY_mb_ka[] = {"¿¨¿¦¿§¿©ØûßÇëÌ"};
const unsigned char PY_mb_kai[] = {"¿ª¿­¿«¿¬¿®ØÜÛîİÜâéâıîøï´ïÇ"};
const unsigned char PY_mb_kan[] = {"¿´¿¯¿²¼÷¿°¿±¿³Ç¶Ù©İ¨ãÛê¬íèî«"};
const unsigned char PY_mb_kang[] = {"¿¹¿µ¿»¿¶¿·¿¸¿ºØøãÊîÖ"};
const unsigned char PY_mb_kao[] = {"¿¼¿¿¿½¿¾åêèàêûîí"};
const unsigned char PY_mb_ke[] = {"¿É¿Ë¿Æ¿Ì¿Í¿Ç¿Å¿Ã¿ÂºÇ¿À¿Á¿Ä¿È¿Ê¿Îà¾á³ã¡äÛæìç¼çæéğë´î§îİï¾ïığâñ½ò¤òò÷Á"};
const unsigned char PY_mb_ken[] = {"¿Ï¿Ğ¿Ñ¿ÒñÌ"};
const unsigned char PY_mb_keng[] = {"¿Ó¿Ôï¬"};
const unsigned char PY_mb_kong[] = {"¿×¿Õ¿Ø¿ÖÙÅáÇóí"};
const unsigned char PY_mb_kou[] = {"¿Ú¿Û¿Ù¿ÜÜÒŞ¢ßµíîóØ"};
const unsigned char PY_mb_ku[] = {"¿à¿â¿İ¿á¿Ş¿ß¿ãØÚÜ¥à·ç«÷¼"};
const unsigned char PY_mb_kua[] = {"¿ç¿ä¿å¿æ¿èÙ¨"};
const unsigned char PY_mb_kuai[] = {"»á¿ì¿é¿ê¿ëØáÛ¦ßàáöä«ëÚ"};
const unsigned char PY_mb_kuan[] = {"¿í¿î÷Å"};
const unsigned char PY_mb_kuang[] = {"¿ö¿ó¿ñ¿ò¿ï¿ğ¿ô¿õÚ²Ú¿Ú÷ÛÛŞÅßÑæşêÜ"};
const unsigned char PY_mb_kui[] = {"¿üÀ£À¡¿÷¿ø¿ù¿ú¿û¿ı¿şÀ¢Ø¸ØÑÙçÚóİŞŞñà­à°ã¦ã´åÓêÒî¥ñùòñóñõÍ"};
const unsigned char PY_mb_kun[] = {"À§À¥À¤À¦ã§ãÍçûï¿õ«öï÷Õ"};
const unsigned char PY_mb_kuo[] = {"À©À¨ÊÊÀ«ÀªèéòÒ"};
const unsigned char PY_mb_l[] = {"ÁËÀ´ÀµÂŞÂÀÁõÂ³ÀîÁÖÀ×ÁÎÂ¬ÁºÀ÷Á®Â½ÁúÀèÂ¹Á¿ÀíÂ·ÀïÂÛÁ½ÀûÁìÁ÷Á¦Á¢ÁÏÁ¬ÀÏÁùÂÖÀàÁĞ"};
const unsigned char PY_mb_la[] = {"À­ÂäÀ²À¯À°À¶À¬À®À±Øİååê¹íÇğø"};
const unsigned char PY_mb_lai[] = {"À´ÀµÀ³áÁáâäµäşêãíùïªñ®ô¥"};
const unsigned char PY_mb_lan[] = {"À¼ÀÃÀ¶ÀÀÀ¸À·À¹ÀºÀ»À½À¾À¿ÀÁÀÂÀÄá°äíé­ìµî½ïçñÜ"};
const unsigned char PY_mb_lang[] = {"ÀËÀÊÀÉÀÇÀÅÀÆÀÈİ¹İõà¥ãÏï¶ïüòë"};
const unsigned char PY_mb_lao[] = {"ÀÏÀÍÂäÀÎÀÔÀÌÂçÀĞÀÑÀÒÀÓÁÊßëáÀèáîîï©ğìñìõ²"};
const unsigned char PY_mb_le[] = {"ÁËÀÖÀÕÀßØìß·ãî÷¦"};
const unsigned char PY_mb_lei[] = {"ÀàÀ×ÀÛÀÕÀİÀáÀØÀÙÀÚÀÜÀŞÀßÙúÚ³àÏæĞçĞéÛñçõª"};
const unsigned char PY_mb_leng[] = {"ÀäÀâÀãÜ¨ã¶"};
const unsigned char PY_mb_li[] = {"ÀíÀïÀûÁ¦Á¢ÀëÀıÀúÁ£ÀåÀñÀîÁ¥ÀèÁ§ÀøÀçÀæÀöÀ÷ÀéÀêÀìÀğÀòÀóÀôÀõÀùÀüÀşÁ¡Á¤Á¨ØªÙ³ÙµÛªÛŞÜÂİ°İñŞ¼ß¿à¦à¬áûã¦äàå¢åÎæ²æËæêçÊèÀèİéöìåíÂîºî¾ï®ğ¿ğİğßòÃòÛó»óÒóöôÏõ·õÈö¨öâ÷¯÷ó"};
const unsigned char PY_mb_lia[] = {"Á©"};
const unsigned char PY_mb_lian[] = {"Á¬ÁªÁ·Á¶Á³Á´Á«Á­Á®Á¯Á°Á±Á²ÁµİüŞÆäòå¥çöé¬éçì¡ñÍñÏó¹öãÁ¢°¸"};
const unsigned char PY_mb_liang[] = {"Á¿Á½Á¸Á¼ÁÁÁºÁ¹Á¾Á»ÁÀÁÂÜ®İ¹é£õÔö¦÷Ë"};
const unsigned char PY_mb_liao[] = {"ÁËÁÏÁÆÁÉÁÅÁÃÁÄÁÇÁÈÁÊÁÌÁÍÁÎŞ¤ŞÍàÚâ²å¼çÔîÉğÓ"};
const unsigned char PY_mb_lie[] = {"ÁĞÁÑÁÒÁÓÁÔÙıÛøŞæßÖä£ôóõñ÷à"};
const unsigned char PY_mb_lin[] = {"ÁÖÁ×ÁÙÁÚÁÜ÷ëÁÕÁØÁÛÁİÁŞÁßİşßøá×âŞãÁåàéİê¥ì¢î¬ôÔõï"};
const unsigned char PY_mb_ling[] = {"ÁìÁíÁãÁîÁéÁëÁåÁäÁèÀâÁêÁàÁáÁâÁæÁçÛ¹ÜßßÊàòãöç±èÚèùê²ñöòÈôáöì"};
const unsigned char PY_mb_liu[] = {"Á÷ÁùÁôÁõÁòÁøÂ½ÁóÁöÁïÁğÁñÂµä¯åŞæòç¸ì¼ìÖï³ïÖğÒöÌ"};
const unsigned char PY_mb_lo[] = {"¿©"};
const unsigned char PY_mb_long[] = {"ÁúÂ¢ÅªÁıÂ¡ÁûÁüÁşÂ£Â¤ÛâÜ×ãñççèĞëÊíÃñª"};
const unsigned char PY_mb_lou[] = {"Â©Â¶Â¥Â¦Â§Â¨ÂªÙÍİäà¶áĞïÎğüñïò÷÷Ã"};
const unsigned char PY_mb_lu[] = {"Â·Â¼Â½Â¯Â¶Â«Â¬Â­Â®Â°Â±Â²Â³Â´ÂµÂ¸Â¹ÂºÂ»Â¾Ûäß£ààãòäËäõåÖè´èÓéÖéñéûê¤ëªëÍïåğµğØóüôµöÔÁùÂÌ"};
const unsigned char PY_mb_luan[] = {"ÂÑÂÒÂÍÂÎÂÏÂĞÙõæ®èïğ½öÇ"};
const unsigned char PY_mb_lue[] = {"ÂÔÂÓï²"};
const unsigned char PY_mb_lun[] = {"ÂÛÂÖÂ×ÂÕÂØÂÙÂÚàğ"};
const unsigned char PY_mb_luo[] = {"ÂäÂŞÂİÂåÂçÂß¿©ÀÓÂÜÂàÂáÂâÂãÂæÙÀÙùÜıŞÛŞûâ¤ãøäğçóé¡ëáíÑïİñ§öÃ"};
const unsigned char PY_mb_lv[] = {"ÂÊÂÉÂÃÂÇÂ¿ÂÀÂÁÂÂÂÄÂÅÂÆÂÈÂËÂÌŞÛãÌéµëöïùñÚÙÍ"};
const unsigned char PY_mb_m[] = {"ÃæÃüÂíÃÏÄÂÃ·ÃÓÃÇÃñÃ«Ã©ÃçÄªÃ»Ã×Ã÷Ã´Ã¿ÃÅÃûÄ¥ÃÀÃÜÂúÄ¶Ä¿ß¼"};
const unsigned char PY_mb_ma[] = {"ÂíÃ´ÂéÂğÂèÂîÂïÂëÄ¦Ä¨ÂêÂìßéáïæÖè¿ó¡"};
const unsigned char PY_mb_mai[] = {"ÂóÂöÂôÂòÂñÂõÛ½İ¤ö²"};
const unsigned char PY_mb_man[] = {"ÂúÂıÂüÂşÂûÂ÷ÂøÂùÃ¡Ü¬á£çÏì×ïÜò©òı÷©÷´"};
const unsigned char PY_mb_mang[] = {"Ã¦Ã¢Ã¤Ã£Ã¥Ã§ÚøäİíËòş"};
const unsigned char PY_mb_mao[] = {"Ã«Ã¬Ã°Ã²Ã³Ã±Ã¨Ã©ÃªÃ­Ã®Ã¯ÙóÜâá¹ã÷è£êÄêóë£ì¸í®î¦ó±÷Ö"};
const unsigned char PY_mb_me[] = {"Ã´÷á"};
const unsigned char PY_mb_mei[] = {"Ã»Ã¿ÃÀÃºÃ¹Ã¸Ã·ÃÃÃ¼ÃµÃ¶Ã½Ã¾ÃÁÃÂÃÄÃÓÃÕİ®áÒâ­ä¼äØé¹ïÑğÌñÇ÷È"};
const unsigned char PY_mb_men[] = {"ÃÇÃÅÃÆŞÑìËí¯îÍ"};
const unsigned char PY_mb_meng[] = {"ÃÏÃÍÃÉÃËÃÎÃÈÃÌÃ¥ÃÊÛÂİùŞ«ãÂëüíæòµòìó·ô»ô¿"};
const unsigned char PY_mb_mi[] = {"Ã×ÃÜÃÔÃÛÃØÃĞÃÑÃÒÃÓÃÕÃÖÃÙÃÚÃİØÂÚ×ŞÂßäà×â¨ãèåµåôëßìòôÍôé÷ã÷ç"};
const unsigned char PY_mb_mian[] = {"ÃæÃŞÃâÃàÃßÃáÃãÃäÃåãæäÅäÏëïíí"};
const unsigned char PY_mb_miao[] = {"ÃçÃëÃèÃíÃîÃéÃêÃìß÷åãç¿çÑèÂíµíğğÅ"};
const unsigned char PY_mb_mie[] = {"ÃğÃïØ¿ßãóºóú"};
const unsigned char PY_mb_min[] = {"ÃñÃôÃòÃóÃõÃöÜåáºãÉãıçÅçäçëíªö¼÷ª"};
const unsigned char PY_mb_ming[] = {"ÃüÃ÷ÃûÃùÃøÃËÃúÚ¤ÜøäéêÔî¨õ¤"};
const unsigned char PY_mb_miu[] = {"ÃıçÑ"};
const unsigned char PY_mb_mo[] = {"ÎŞÄ¥Ã»ÍòÄ©Ä£Ä¤ÃşÄ«Ä¦ÂöÄªÄ¨Ã°Ä¬ºÑºÙÄ¡Ä¢Ä§Ä­Ä®Ä¯Ä°ÚÓÜÔİëâÉæÆéâïÒï÷ñ¢ñòõöõø"};
const unsigned char PY_mb_mou[] = {"Ä³Ä±Ä²Ù°ßèçÑíøòÖòúöÊ"};
const unsigned char PY_mb_mu[] = {"Ä¶Ä¿Ä¾Ä¸Ä¹Ä£Ä»ÄÁÄ·ÄÂÀÑÄ²Ä´ÄµÄºÄ¼Ä½ÄÀØïÛéÜÙãåë¤îâ"};
const unsigned char PY_mb_n[] = {"ÄêÄÜÄÚÄÇÄôÅ©ÄãÄÏÄÑÄØÅ®ÄàÄÄÄÃÄîÅ£ÄşÄÔÕ³ÄáÄÉàÅ"};
const unsigned char PY_mb_na[] = {"ÄÇÄÏÄÄÄÃÄÉÄØÄÆÄÅÄÈŞàëÇïÕñÄ"};
const unsigned char PY_mb_nai[] = {"ÄÍÄÄÄÌÄËÄÊÄÎØ¾ÜµİÁèÍ"};
const unsigned char PY_mb_nan[] = {"ÄÏÄÑÄĞà«àîàïéªëîòïôö"};
const unsigned char PY_mb_nang[] = {"ÄÒß­àìâÎêÙ"};
const unsigned char PY_mb_nao[] = {"ÄÔÄÖÅ¬ÄÓÄÕÄ×Ø«ÛñßÎâ®è§íĞîóòÍ"};
const unsigned char PY_mb_ne[] = {"ÄØÄÄÄÅÚ«"};
const unsigned char PY_mb_nei[] = {"ÄÚÄÇÄÄÄÙ"};
const unsigned char PY_mb_nen[] = {"ÄÛí¥"};
const unsigned char PY_mb_neng[] = {"ÄÜ"};
const unsigned char PY_mb_ni[] = {"ÄãÄàÄáÄØÄæÄâÄòÄİÄŞÄßÄäÄåÄçÙ£Ûèâ¥âõêÇì»ìòí«íşîêöò"};
const unsigned char PY_mb_nian[] = {"ÄêÄîÕ³ÄèÄéÄëÄìÄíÄğØ¥Ûşéığ¤öÓöó"};
const unsigned char PY_mb_niang[] = {"ÄïÄğ"};
const unsigned char PY_mb_niao[] = {"ÄòÄñÄçÜàæÕëåôÁ"};
const unsigned char PY_mb_nie[] = {"ÄøÄöÄùÄóÄôÄõÄ÷Ø¿ÚíŞÁà¿ò¨ô«õæ"};
const unsigned char PY_mb_nin[] = {"Äúí¥"};
const unsigned char PY_mb_ning[] = {"ÄşÄıÅ¡ÄûÄüÅ¢ØúÜÑßÌå¸ñ÷"};
const unsigned char PY_mb_niu[] = {"Å£Å¤Å¥Å¦ŞÖáğâîæ¤"};
const unsigned char PY_mb_nong[] = {"Å©ÅªÅ¨Å§Ù¯ßæ"};
const unsigned char PY_mb_nou[] = {"ññ"};
const unsigned char PY_mb_nu[] = {"Å¬Å«Å­åóæÀæÛæå"};
const unsigned char PY_mb_nuan[] = {"Å¯"};
const unsigned char PY_mb_nuo[] = {"ÅµÄÈÅ²Å³Å´ÙĞŞùßöï»"};
const unsigned char PY_mb_nv[] = {"Å®í¤îÏô¬"};
const unsigned char PY_mb_nve[] = {"Å°Å±"};
const unsigned char PY_mb_o[] = {"Å¶à¸àŞÅ·Å¼Å¸Å¹ÅºÅ»âæñî"};
const unsigned char PY_mb_ou[] = {"ÇøÅ·Å¼Å¸Å¹ÅºÅ»Å½Ú©âæê±ñî"};
const unsigned char PY_mb_p[] = {"ÅúÆ½ÅĞÅËÅíÅÓÅáÆÑÅäÆ¬ÅÉÆÆÅÅÆ·Æ¤ÅçÅÌÆ«ÆÀÅÂÅàÆÕÆ¶ÅÜÅÚ"};
const unsigned char PY_mb_pa[] = {"ÅÂÅÉÅÀÅÁ°Ç°ÒÅ¾Å¿ÅÃİâèËîÙóá"};
const unsigned char PY_mb_pai[] = {"ÅÉÅÅÅÄÅÆÆÈßßÅÇÅÈÙ½İå"};
const unsigned char PY_mb_pan[] = {"ÅĞÅÌÅÑ·¬ÅËÅÊÅÍÅÎÅÏÅÖŞÕãİãúñÈñáó´õç"};
const unsigned char PY_mb_pang[] = {"ÅÔ°ò°õÅÒÅÓÅÕÅÖáİäèåÌó¦"};
const unsigned char PY_mb_pao[] = {"ÅÜÅÚÅÙÅ×ÅİÅØÅÛŞËáóâÒëãğå"};
const unsigned char PY_mb_pei[] = {"ÅäÅàÅãÅßÅŞÅáÅâÅåÅæàÎàúì·ïÂõ¬ö¬"};
const unsigned char PY_mb_pen[] = {"ÅçÅèäÔ"};
const unsigned char PY_mb_peng[] = {"ÅöÅïÅîÅóÅõÅòÅéÅêÅëÅìÅíÅğÅñÅôÜ¡àØâñó²"};
const unsigned char PY_mb_pi[] = {"ÅúÆ¤»µ·ñ±ÙÅ÷Æ¢Æ£ÅøÅùÅûÅüÅıÅşÆ¡Æ¥Æ¦Æ§Æ¨Æ©Ø§ØòÚéÚğÚüÛ¯ÛÜÛıÜ±ÜÅß¨ßÁàèâÏäÄæÇç¢èÁê¶î¢î¼îëñ±ñÔñâò·òçõù"};
const unsigned char PY_mb_pian[] = {"Æ¬Æ«±ãÆªÆ­±âÚÒæéçÂêúëİôæõä"};
const unsigned char PY_mb_piao[] = {"Æ±Æ¯ÆÓÆ®Æ°Øâİ³àÑæÎæôçÎéèî©óª"};
const unsigned char PY_mb_pie[] = {"Æ²Æ³Ø¯ÜÖë­"};
const unsigned char PY_mb_pin[] = {"Æ·Æ¶ÆµÆ´Æ»Æ¸ŞÕæ°æÉé¯êòò­"};
const unsigned char PY_mb_ping[] = {"Æ½ÆÀÆ¿Æ¾Æ»·ëÆ¹ÆºÆ¼ÆÁÙ·æ³èÒöÒÚ¢"};
const unsigned char PY_mb_po[] = {"ÆÆÆÈÆÂÆÃ·±ÆÓ²´ÆÄÆÅÆÇÆÉØÏÚéÛ¶ãøçêê·îÇîŞîàğ«óÍ"};
const unsigned char PY_mb_pou[] = {"ÆÊÙöŞå"};
const unsigned char PY_mb_pu[] = {"ÆÕÆ×ÆËÆÒÆÌ±©ÆÏÆÓÆÑ±¤¸¬ÆÍÆÎÆĞÆÔÆÖÆØÆÙÙéàÛäßå§è±ë«ïäïèõë"};
const unsigned char PY_mb_q[] = {"ÆğÇ°ÆäÇ®ÇÇÆëÆîÇØÇüÇñÆİöÄÈ¥ÆøÈ«ÆÚÈºÇéÇøÇ¿ÇĞÈ¡ÆßÇóÇåÈ·È¨ÇÒÇà"};
const unsigned char PY_mb_qi[] = {"ÆğÆäÆøÆÚÆßÆ÷ÆëÆæÆûÆóÆáÆÛÆìÆèÆôÆúÆç»ü¼©ÆÜÆİÆŞÆàÆâÆãÆåÆéÆêÆíÆîÆïÆñÆòÆõÆöÆùÆüÆıØ½ØÁÙ¹ÛßÜ»ÜÎÜùİ½İÂİİŞ­àÒá¨áªá«ãàä¿æëç²ç÷çùè½èçéÊêÈì¥ì÷í¬íÓñıòÓòàôëôìõè÷¢÷è"};
const unsigned char PY_mb_qia[] = {"¿¨Ç¡ÆşÇ¢Ù¤İÖñÊ÷Ä"};
const unsigned char PY_mb_qian[] = {"Ç°Ç§Ç®Ç³Ç©Ç¨Ç¦Ç±Ç£Ç¯ÏËÇ´Ç¤Ç¥ÇªÇ«Ç¬Ç­Ç²ÇµÇ¶Ç·Ç¸Ù»ÙİÚäÜ·ÜÍÜçİ¡Şçá©ã¥ã»å¹åºå½ç×èıêùëÉí©îÔò¯óèóé"};
const unsigned char PY_mb_qiang[] = {"Ç¿½«Ç¹ÇÀÇ½Ç»ÇºÇ¼Ç¾ãŞæÍéÉê¨ìÁïºïÏïêñßòŞôÇõÄ"};
const unsigned char PY_mb_qiao[] = {"ÇÅÇÆ¿ÇÇÉÇÃÇÇ½¶ÇÁÇÂÇÄÇÈÇÊÇËÇÌÇÍÇÎÇÏÈ¸ØäÚ½ÚÛÜñá½ã¸ã¾çØéÔíÍõÎ÷³"};
const unsigned char PY_mb_qie[] = {"ÇĞÇÒÆöÇÑÇÓÇÔÙ¤Û§ã«ã»æªêüïÆóæ"};
const unsigned char PY_mb_qin[] = {"Ç×ÇÖÇÚÇØÇÕÇÙÇÛÇÜÇİÇŞÇßÜËŞìßÄàºàßâÛäÚéÕï·ñæñûòûôÀ"};
const unsigned char PY_mb_qing[] = {"ÇéÇåÇàÇáÇãÇëÇ×ÇìÇâÇçÇäÇæÇèÇêÜÜàõéÑíàòßóÀóäôìö¥öë÷ô"};
const unsigned char PY_mb_qiong[] = {"ÇîÇíÚöÜäñ·òËóÌõ¼öÆ"};
const unsigned char PY_mb_qiu[] = {"ÇóÇòÇïÇğ³ğ¹êÇñÇôÇõÇöÙ´ÛÏáìäĞåÏåÙé±êäò°òÇòøôÃôÜöú÷ü"};
const unsigned char PY_mb_qu[] = {"È¥ÇøÈ¡ÇúÇÒÇşÇüÇ÷ÇıÈ¤ÇùÇûÈ¢È£ĞçÚ°Û¾ÜÄŞ¡Ş¾á«áéãÖè³êïë¬ëÔìîíáğ¶ñ³òĞó½ôğöÄ÷ñ"};
const unsigned char PY_mb_quan[] = {"È«È¨È¦È°ÈªÈ©È§È¬È­È®È¯Ú¹Üõãªç¹éúî°îıòéóÜ÷Ü"};
const unsigned char PY_mb_que[] = {"È·È´È±È²È³ÈµÈ¶È¸ã×ãÚí¨"};
const unsigned char PY_mb_qun[] = {"ÈºÈ¹åÒ"};
const unsigned char PY_mb_r[] = {"ÈËÈçÈÕÈîÈÙÈÏÈëÈÎÈÈÈ»ÈİÈÃÈóÈõÈ¼È¾ÈÀÈÆÈÊÈÜÈĞÈÔ"};
const unsigned char PY_mb_ran[] = {"È»È¼È¾È½ÜÛòÅ÷×"};
const unsigned char PY_mb_rang[] = {"ÈÃÈÀÈÂÈ¿ÈÁìüğ¦"};
const unsigned char PY_mb_rao[] = {"ÈÆÈÅÈÄÜéæ¬èã"};
const unsigned char PY_mb_re[] = {"ÈÈÈôÈÇ"};
const unsigned char PY_mb_ren[] = {"ÈËÈÏÈÎÈÊÈĞÈÌÈÉÈÍÈÑÈÒØğÜóİØâ¿éíïşñÅ"};
const unsigned char PY_mb_reng[] = {"ÈÔÈÓ"};
const unsigned char PY_mb_ri[] = {"ÈÕ"};
const unsigned char PY_mb_rong[] = {"ÈİÈÜÈÙÈÛÈÚÈŞÈÖÈ×ÈØÈßáÉáõéÅëÀòî"};
const unsigned char PY_mb_rou[] = {"ÈâÈàÈáôÛõå÷·"};
const unsigned char PY_mb_ru[] = {"ÈçÈëÈåÈéÈãÈäÈæÈèÈêÈìİêŞ¸àéä²äáå¦çÈï¨ñàò¬"};
const unsigned char PY_mb_ruan[] = {"ÈíÈîëÃ"};
const unsigned char PY_mb_rui[] = {"ÈğÈñÈïÜÇŞ¨èÄî£ò¸"};
const unsigned char PY_mb_run[] = {"ÈóÈò"};
const unsigned char PY_mb_ruo[] = {"ÈõÈôÙ¼óè"};
const unsigned char PY_mb_s[] = {"ÊÇÉÏÊ±ËïËÎËÕÉòËÚÊ©ÉÛÙÜÉêÊ¦Ë§É³ËåÊ¯Ê·ÉúËµÉçÉîË®ËùÈıÊ®Ê¹ÊÂÊıËÄÊµÉÙÊ½É½ÊÖ"};
const unsigned char PY_mb_sa[] = {"ÈöÈøÈ÷Ø¦ØíêıëÛìª"};
const unsigned char PY_mb_sai[] = {"ÈûË¼ÈüÈùÈúàç"};
const unsigned char PY_mb_san[] = {"ÈıÉ¢ÈşÉ¡âÌë§ôÖ"};
const unsigned char PY_mb_sang[] = {"É£É¥É¤Şúíßòª"};
const unsigned char PY_mb_sao[] = {"É¨ÉÒÉ¦É§É©Ü£çÒçØëığşöş"};
const unsigned char PY_mb_se[] = {"É«ÈûÉªÉ¬ØÄï¤ğ£"};
const unsigned char PY_mb_sen[] = {"É­"};
const unsigned char PY_mb_seng[] = {"É®"};
const unsigned char PY_mb_sh[] = {"ÊıÏÃÉ½É¾ÉÌÉÙÕÙÉçÉèÉõÉùÊĞÊ¦Ê¼ÊÖÊÕÊØÊéÊôÊõÊäË«Ë®Ë­Ë¯Ë°Ë³"};
const unsigned char PY_mb_sha[] = {"É³É±É°É¶É´É¯É²ÉµÉ·É¼ÏÃßşàÄêıì¦ï¡ğğôÄö®öè"};
const unsigned char PY_mb_shai[] = {"É«É¸É¹"};
const unsigned char PY_mb_shan[] = {"É½µ¥ÉÁÉÆÉºÉÈÉÂµ§É»É¼É¾É¿ÉÀÉÃÉÄÉÅÉÇÉÉÕ¤ØßÚ¨Û·ÛïÜÏäúæ©æÓæóëşìøîÌğŞóµô®õÇ÷­"};
const unsigned char PY_mb_shang[] = {"ÉÏÉÌÉËÉĞÌÀÉÊÉÍÉÎÉÑÛğç´éäìØõü"};
const unsigned char PY_mb_shao[] = {"ÉÙÉÕÉÔÉÜÕÙÉÚÉÒÇÊÉÓÉÖÉ×ÉØÉÛÛ¿Üæäûè¼òÙóâô¹"};
const unsigned char PY_mb_she[] = {"ÉçÉèÉäÉãÉàÕÛÉæÉáÉßÉİÉŞÉâÉåØÇÙÜß¡â¦äÜì¨î´÷ê"};
const unsigned char PY_mb_shen[] = {"ÉîÉíÉñÊ²Éì²ÎÉõÉøÉòÉöÉóÉêÉ÷ÉéÉëÉïÉğÉôÚ·ÚÅİ·İØßÓäÉé©ëÏïòò×"};
const unsigned char PY_mb_sheng[] = {"ÉúÊ¤ÉùÊ¡ÉıÊ¢ÉşÊ£Ê¥Éü³ËÉûáÓäÅêÉíòóÏ"};
const unsigned char PY_mb_shi[] = {"ÊÇÊ±Ê®Ê¹ÊÂÊµÊ½Ê¶ÊÀÊÔÊ¯Ê²Ê¾ÊĞÊ·Ê¦Ê¼Ê©Ê¿ÊÆÊªÊÊÊ³Ê§ÊÓÊÒÊÏËÆÊ´Ê«ÊÍÖ³Ê°ÊÎÊ»³×Ê¨Ê¬Ê­Ê¸ÊºÊÁÊÃÊÄÊÅÊÈÊÉÊËÊÌÊÑĞêÖÅÚÖÛõİªİéß±éøêÛìÂîæó§óÂóßõ§õ¹öåöõìêâ»"};
const unsigned char PY_mb_shou[] = {"ÊÖÊÜÊÕÊ×ÊØÊìÊÚÊÙÊŞÊÛÊİá÷ç·ô¼"};
const unsigned char PY_mb_shu[] = {"ÊıÊéÊ÷ÊôÊõÊäÊöÊìÊøÊóÊèÊâÊæÊßÊíÊåÊğÊàÊáÊãÊçÊêÊëÊîÊïÊñÊòÊùÊúÊûÊüÊşË¡ÓáØ­Ù¿ÛÓİÄŞóãğäøæ­ç£ë¨ëòì¯ïø"};
const unsigned char PY_mb_shua[] = {"Ë¢Ë£à§"};
const unsigned char PY_mb_shuai[] = {"Ë¥Ë§Ë¤Ë¦ó°"};
const unsigned char PY_mb_shuan[] = {"Ë¨Ë©ãÅäÌ"};
const unsigned char PY_mb_shuang[] = {"Ë«ËªË¬ãñæ×"};
const unsigned char PY_mb_shui[] = {"Ë®ËµË­Ë¯Ë°"};
const unsigned char PY_mb_shun[] = {"Ë³Ë±Ë²Ë´"};
const unsigned char PY_mb_shuo[] = {"ËµÊıË¶Ë·Ë¸İôŞ÷åùéÃîå"};
const unsigned char PY_mb_si[] = {"ËÄË¼ËÀË¹Ë¿ËÆË¾ËÇÊ³Ë½²ŞËºË»ËÁËÂËÃËÅËÈØËÙ¹ÙîÛÌßĞãáãôäùæ¦æáçÁìëïÈğ¸ñêòÏóÓ"};
const unsigned char PY_mb_song[] = {"ËÉËÍËÎËÌËÊËËËÏËĞÚ¡İ¿áÂáÔâìã¤äÁñµ"};
const unsigned char PY_mb_sou[] = {"ËÑËÒËÓËÔÛÅŞ´à²àÕâÈäÑì¬î¤ïËòô"};
const unsigned char PY_mb_su[] = {"ËØËÙËÕËÜËõË×ËßËŞËàËÖËÚËÛËİÙíÚÕİøà¼ãºä³óùö¢öÕ"};
const unsigned char PY_mb_suan[] = {"ËãËáËââ¡"};
const unsigned char PY_mb_sui[] = {"ËæËëËéËäËêÄòËåËçËèËìËíËîÚÇİ´å¡åäìİíõî¡"};
const unsigned char PY_mb_sun[] = {"ËğËïËñİ¥áøâ¸é¾öÀ"};
const unsigned char PY_mb_suo[] = {"ËùËõËøË÷É¯ËòËóËôËößïàÂàÊæ¶èøêıíüôÈ"};
const unsigned char PY_mb_t[] = {"ËûÍ¬ËüÌÆÌ·ÌÕÌÀÍ¯Í¿ÌïÙ¡ÌáÌâÌåÍ¼ÌìÍ·Í¨ÌõÌØÍÁÍÅÍ³Ì¨ËıÌ«Ì×Ìú"};
const unsigned char PY_mb_ta[] = {"ËûËüËıËşÌ¤ËúÌ¡Ì¢Ì£ÍØàªãËäâäğåİé½í³îèõÁ÷£"};
const unsigned char PY_mb_tai[] = {"Ì¨Ì«Ì¬Ì¥Ì§Ì©Ì¦ÌªÌ­Û¢Ş·ß¾ææëÄìÆîÑõÌöØ"};
const unsigned char PY_mb_tan[] = {"Ì¸Ì¼Ì½Ì¿µ¯Ì¹Ì°Ì²Ì®Ì¯Ì±Ì³Ì´ÌµÌ¶Ì·ÌºÌ»Ì¾Û°å£ê¼ìşîãïÄïâ"};
const unsigned char PY_mb_tang[] = {"ÌÃÌÇÌÆÌÁÌÀÌÂÌÄÌÅÌÈÌÉÌÊÌËÌÌÙÎàûäçè©éÌï¦ïÛñíó¥ó«ôÊõ±"};
const unsigned char PY_mb_tao[] = {"Ì×ÌÖÌÓÌÕÌÑÌÒÌÍÌÎÌÏÌĞÌÔØ»ß¶ßûä¬èºìâ÷Ò"};
const unsigned char PY_mb_te[] = {"ÌØß¯ìıí«ï«"};
const unsigned char PY_mb_teng[] = {"ÌÚÌÛÌÙÌÜëø"};
const unsigned char PY_mb_ti[] = {"ÌáÌâÌåÌæÌİÌèÌŞÌßÌàÌãÌäÌçÌéÌêÌëÙÃÜèã©åÑç°ç¾ğÃñÓõ®ŞĞ"};
const unsigned char PY_mb_tian[] = {"ÌìÌïÌíÌîÌğµèÌñÌòÌóŞİãÃãÙéåî±"};
const unsigned char PY_mb_tiao[] = {"Ìõµ÷ÌøÌôÌöÌ÷Ù¬Üæìöñ»òèóÔôĞö¶öæ÷Ø"};
const unsigned char PY_mb_tie[] = {"ÌúÌùÌûİÆ÷Ñ"};
const unsigned char PY_mb_ting[] = {"ÌıÍ£Í¥Í¦Í¢ÌüÌşÍ¡Í¤Í§ÜğİãæÃèèî®îúòÑöª"};
const unsigned char PY_mb_tong[] = {"Í¬Í¨Í³Í­Í´Í²Í¯Í°¶±¶²Í©ÍªÍ«Í®Í±Ù¡ÙÚÛíÜíàÌá¼âúäüíÅ"};
const unsigned char PY_mb_tou[] = {"Í·Í¶Í¸Íµî×÷»"};
const unsigned char PY_mb_tu[] = {"Í¼ÍÁÍ»Í¾Í½Í¹Í¿ÍÂÍÃÍÀÍºÜ¢İ±İËîÊõ©"};
const unsigned char PY_mb_tuan[] = {"ÍÅÍÄŞÒåèî¶Í¼°¸"};
const unsigned char PY_mb_tui[] = {"ÍÆÍËÍÈÍÇÍÉÍÊß¯ìÕ"};
const unsigned char PY_mb_tun[] = {"¶ÚÍÊÍÌÍÍÍÎÙÛâ½êÕëà"};
const unsigned char PY_mb_tuo[] = {"ÍÑÍÏÍĞÍ×ÍÖ¶æÆÇÍÒÍÓÍÔÍÕÍØÍÙØ±Ù¢ÛçâÕãûèØèŞéÒíÈîèóêõ¢õÉö¾"};
const unsigned char PY_mb_w[] = {"ÎªÎÒÎŞÍõÍôÎÀÎºÎâÎ¤ÎÅÎÚÚùÎÌÎÂÎéÎÊÍâÎåÎ»ÎïÎ¯ÍêÎÂÍòÎÄÍùÎäÎñÎ§ÎóÍí"};
const unsigned char PY_mb_wa[] = {"ÍßÍÚ°¼ÍÛÍÜÍİÍŞÍàØôæ´ëğ"};
const unsigned char PY_mb_wai[] = {"ÍâÍááË"};
const unsigned char PY_mb_wan[] = {"ÍêÍòÍíÍäÍëÍçÍåÍìÍæÂûÃäÍãÍèÍéÍîÍïÍğÍñÍóØàÜ¹İ¸İÒæıçºçşëäîµòê"};
const unsigned char PY_mb_wang[] = {"ÍùÍõÍûÍøÍüÍıÍöÍúÃ¢ÍôÍ÷ØèŞÌã¯éş÷Í"};
const unsigned char PY_mb_wei[] = {"ÎªÎ»Î¯Î§Î¬Î¨ÎÀÎ¢Î°Î´ÍşÎ£Î²Î½Î¹Î¶Î¸ÎºÎ±ÒÅÎ¥Î¤Î·Î³Î¡Î¦Î©Î«Î­Î®ÎµÎ¼Î¾Î¿ÙËÚÃÚñÚóÛ×İÚŞ±àíàøá¡áËáÍâ«â¬ãÇãíä¢ä¶åÔæ¸çâè¸ê¦ì¿ìĞğôôºöÛ"};
const unsigned char PY_mb_wen[] = {"ÎÊÎÂÎÄÎÈÎÆÎÅÎÃÎÁÎÇÎÉØØãÓãëè·ö©Ùï"};
const unsigned char PY_mb_weng[] = {"ÎËÎÌÎÍİîŞ³"};
const unsigned char PY_mb_wo[] = {"ÎÒÎÕÎÑÎÏÎĞÎÖÎÎÎÔÎÓÙÁİ«à¸á¢ä×ë¿íÒö»"};
const unsigned char PY_mb_wu[] = {"ÎŞÎåÎïÎäÎñÎóÎéÎèÎÛÎòÎíÎç¶ñÎİÎÚÎâÍöÎÜÎÙÎ×ÎØÎßÎàÎáÎãÎæÎêÎëÎìÎîÎğØ£ØõÚãÚùÛØÜÌßíâĞâäâèä´å»åÃåüæÄæğè»êõì¶ìÉğÄğÍğíòÚöÈ÷ù"};
const unsigned char PY_mb_x[] = {"Ñ§ÏÂĞĞĞíĞìĞÏÏôĞ¤Ñ¦ĞÜ½âÏîÏ°ÙşĞ¡ÏßĞÔĞÂĞ©ÏëĞÄÏòÏàÏµÏ¯ÏÖÎ÷ĞŞÏóÏÈĞÎ"};
const unsigned char PY_mb_xi[] = {"ÏµÏ¯Î÷Ï°Ï¸ÎüÎöÏ²Ï´Ï³Ï¡Ï·Ï¶Ï£Ï¢Ï®ÎıÏ©ÎşÏ¤Ï§ÏªÀ°ÆÜÎôÎõÎøÎùÎúÎûÏ¥Ï¦Ï¨Ï«Ï¬Ï­Ï±ÙÒÙâÚôÛ­Üçİ¾İßİûŞÉßñáãâ¾ãÒä»äÀåïæÒçôéØêØêêì¤ì¨ìäìùìûğªñ¶ñÓòáó£ó¬ôªô¸ôËôÑôâõµõè÷û"};
const unsigned char PY_mb_xia[] = {"ÏÂÏÄÏÅÏÁÏ¼»£Ï¹ÏºÏ»Ï½Ï¾Ï¿ÏÀÏÃßÈáòåÚè¦èÔíÌğıóÁ÷ï"};
const unsigned char PY_mb_xian[] = {"ÏßÏÖÏÈÏØÏŞ¼ûÏÔÏÊÏ×ÏÕÏİÏÜÏËÏ´ÏÆÏÒÏ³ÏÙÏÇÏÉÏÌÏÍÏÎÏÏÏĞÏÑÏÓÏÚÏÛÙşÜÈİ²Şºá­áıåßæµë¯ìŞììğÂğïò¹óÚôÌõ£õĞõÑö±"};
const unsigned char PY_mb_xiang[] = {"ÏëÏòÏàÏóÏìÏîÏäÏç½µÏãÏñÏêÏğÏíÏæÏáÏâÏåÏèÏéÏïÜ¼İÙâÃâÔæøç½ó­öß÷Ï"};
const unsigned char PY_mb_xiao[] = {"Ğ¡ÏûÏ÷Ğ§Ğ¦Ğ£ÏúÏõÏôĞ¤Ğ¢ÏöÏøÏùÏüÏıÏşĞ¥ßØáÅäìåĞæçç¯èÉèÕòÙóãóï÷Ì"};
const unsigned char PY_mb_xie[] = {"Ğ©½âĞ´Ğ±Ğ»Ğ­ĞµÒ¶ÑªĞ¶Ğ¼Ğ¬ĞªĞ°Ğ²Ğ·Ğ¹ĞºÆõĞ¨Ğ«Ğ®Ğ¯Ğ³Ğ¸ÙÉÙôÛÄÛÆŞ¯ß¢â³âİäÍå¬åâç¥çÓé¿éÇò¡õó"};
const unsigned char PY_mb_xin[] = {"ĞÂĞÄĞÅĞ¿Ğ¾ĞÁÑ°ĞÀĞ½ĞÃĞÆØ¶Ü°ê¿ì§ïâöÎ"};
const unsigned char PY_mb_xing[] = {"ĞĞĞÔĞÎĞÍĞÇĞËÊ¡ĞÑĞÕĞÒĞÈĞÉĞÊĞÌĞÏĞÓÚêÜôÜşß©â¼ã¬íÊ"};
const unsigned char PY_mb_xiong[] = {"ĞÛĞØĞÖĞ×ĞÜĞÙĞÚÜº"};
const unsigned char PY_mb_xiu[] = {"ĞŞĞâĞİĞäĞãĞàËŞ³ôĞßĞáĞåßİá¶âÊâÓäåğ¼õ÷÷Û"};
const unsigned char PY_mb_xu[] = {"ĞøĞíĞëĞèĞòĞéĞõĞóĞğĞîĞ÷ĞìĞæĞçĞêĞïĞñĞôĞöÓõÚ¼ÛÃÛ×Ş£äªä°äÓçïèòìãí¹íìñãôÚõ¯"};
const unsigned char PY_mb_xuan[] = {"Ñ¡ĞıĞûĞüĞşÈ¯ĞùĞúÑ¢Ñ£Ñ¤ÙØÚÎİæŞïãùäÖäöè¯é¸êÑìÅìÓíÛîçïàğç"};
const unsigned char PY_mb_xue[] = {"Ñ§ÑªÏ÷Ñ©Ñ¨Ñ¥Ñ¦ÚÊàåí´õ½÷¨"};
const unsigned char PY_mb_xun[] = {"ÑµÑ®Ñ¸Ñ¶Ñ°Ñ­Ñ²»ç¿£Ñ«Ñ¬Ñ¯Ñ±Ñ³Ñ´Ñ·ÙãÛ¨Û÷Ü÷Ş¦Ş¹á¾áßâ´âşä­ä±êÖñ¿õ¸öàåæ"};
const unsigned char PY_mb_y[] = {"Ò»ÓĞÑîÑÏÓàÑÖÒ¦ÑÕÔ¬ÑàÓáÔÀã¢ÀÖÒ¶ÑòÒüÓÈÓÃÒªÒÔÓÚÒ²ÑùÒåÓëÓÉÑ¹Ô±ÔÂÒâÓÖÔËÔ­ÓÍÑé"};
const unsigned char PY_mb_ya[] = {"Ñ¹ÑÇÑ½ÑÀÑ¿ÑÅÑÁÑ¼Æ¥ÑºÑ»Ñ¾ÑÂÑÃÑÄÑÆÑÈØóÛëŞëá¬åÂæ«çğèâë²í¼íığé"};
const unsigned char PY_mb_yan[] = {"ÑéÑĞÑÏÑÕÑÛÑÔÑÎÑİÑÒÑØÑÌÑÓÑÚÑçÑ×ÑàÑÜÇ¦ÑÉÑÊÑËÑÍÑÑÑÖÑÙÑŞÑßÑáÑâÑãÑäÑåÑæÑèÒóØÉØÍØßÙ²ÙÈÙğÚİÚçÛ±Û³Ü¾İÎáÃâûãÆãÕäÎäÙåûæÌçüéÜêÌëÙëçìÍî»óÛõ¦÷Ê÷Ğ÷ú"};
const unsigned char PY_mb_yang[] = {"ÑùÑøÑõÑïÑóÑôÑòÑíÑëÑîÑöÑêÑìÑğÑñÑ÷Ñúáàâóãóì¾ìÈí¦òÕ÷±"};
const unsigned char PY_mb_yao[] = {"ÒªÒ©Ô¼Ò¡ÑüÒ§ÑûÒ«ÌÕ½ÄÅ±ÑıÑşÒ¢Ò£Ò¤Ò¥Ò¦Ò¨Ô¿Ø²Ø³ßºáÊáæçÛçòèÃé÷ê×ëÈï¢ğÎñºôí÷¥"};
const unsigned char PY_mb_ye[] = {"Ò²ÒµÒ³Ò¶ÒºÒ¹Ò°Ò¯Ò±Ğ°»©ÑÊÒ¬Ò­Ò®Ò´Ò·Ò¸×§ØÌÚËÚşŞŞêÊìÇîô"};
const unsigned char PY_mb_yi[] = {"Ò»ÒÔÒåÒâÒÑÒÆÒ½ÒéÒÀÒ×ÒÒÒÕÒæÒìÒËÒÇÒÚÒÅÒÁÒÛÒÂÒÉÒàÒêÒíÒëÒÖÒäÒßÒ¼Ò¾Ò¿ÒÃÒÄÒÈÒÊÒÌÒÍÒÎÒÏÒĞÒÓÒØÒÙÒÜÒİÒŞÒáÒãÒçÒèÒîÒïØ×ØæØîØıÙ«Ú±ÛİÛüÜ²ÜÓÜèŞ²ŞÄŞÈŞÚß®ß½ß×ßŞàÉàæá»áÚâ¢âÂâøâùã¨äôåÆæäçËéìéóêİì¥ì½ìÚíôîÆï×ïîğêğùñ¯ñ´òæô¯ôàôèôı÷ğñÂ"};
const unsigned char PY_mb_yin[] = {"ÒòÒıÒõÓ¡ÒôÒøÑÌÒşÒûÒñÒğÒóÒöÒ÷ÒùÒúÒüØ·Û´ÛßÛóÜ§ÜáßÅà³áşâ¹ä¦äÎë³î÷ñ«ñ¿ò¾ö¯ö¸"};
const unsigned char PY_mb_ying[] = {"Ó¦Ó°Ó²ÓªÓ¢Ó³Ó­Ó£Ó¤Ó¥Ó§Ó¨Ó©Ó«Ó¬Ó®Ó¯Ó±ÙøÛ«ÜãÜşİºİÓİöŞüàÓâßäŞäëå­çøè¬éºëôğĞñ¨ò£ó¿"};
const unsigned char PY_mb_yo[] = {"ÓıÓ´à¡"};
const unsigned char PY_mb_yong[] = {"ÓÃÓÂÓÀÓµÓ¿Ó¼Ó¹Ó¶Ó·Ó¸ÓºÓ»Ó½Ó¾ÓÁÙ¸ÛÕÜ­à¯ã¼çßïŞğ®÷«÷Ó"};
const unsigned char PY_mb_you[] = {"ÓĞÓÉÓÖÓÍÓÒÓÑÓÅÓ×ÓÎÓÈÓÕÓÌÓÄÓÆÓÇÓÊÓËÓÏÓÓÓÔØÕØüÙ§İ¬İ¯İµŞÌßÏàóå¶èÖéàë»îğğàòÄòÊòöòøôíöÏ÷î÷ø"};
const unsigned char PY_mb_yu[] = {"ÓÚÓëÓıÓãÓêÓñÓàÓöÔ¤ÓòÓïÓúÓæÓèÓğÓŞÓù¹ÈÓûÓîÎµÎ¾ÓØÓÙÓÛÓÜÓİÓßÓáÓâÓäÓåÓçÓéÓìÓíÓóÓôÓõÓ÷ÓøÓüÓşÔ¡Ô¢Ô£Ô¥Ô¦ÖàØ®Ø¹ØñÙ¶ÚÄÚÍİÇİÒİ÷ŞíàôàöáÎáüâÀâÅâ×ãĞå÷åıæ¥æúè¤êÅêìëéëòì£ì¶ìÏìÙìÛí²îÚğÁğÖğõğöñ¾ñÁòâòõóÄô§ô¨ö§ö¹í±"};
const unsigned char PY_mb_yuan[] = {"Ô±Ô­Ô²Ô´ÔªÔ¶Ô¸ÔºÔµÔ®Ô°Ô¹Ô§Ô¨Ô©Ô«Ô¬Ô¯Ô³Ô·ÛùÜ«Ü¾Şòà÷ãäæÂè¥éÚë¼íóğ°ó¢óîö½"};
const unsigned char PY_mb_yue[] = {"ÔÂËµÔ½Ô¼ÀÖÔ¾Ô»ÔÄÔ¿ÔÀÔÁÔÃÙßå®èİéĞë¾îá"};
const unsigned char PY_mb_yun[] = {"ÔËÔ±ÔÆÔÈ¾ùÔÊÔĞÔÅÔÇÔÉÔÌÔÍÔÎÔÏÛ©Ü¿áñã¢ã³ç¡è¹éæêÀëµìÙ"};
const unsigned char PY_mb_z[] = {"ÔÚÕâÖ÷ÖĞÕÅÕÂÕÔÔøÖ£ÖÜ×ŞÖìÖÖ×Å×£Õç×¯×ÚÕ²ê°×æ×óÕ¹ÕùÕ½×÷ÕıÖ®ÖÆÖØ×éÖÎÖ»×Ó×ÔÖÊÖ¸"};
const unsigned char PY_mb_za[] = {"ÔÓÔúÔÛÔÑÔÒÕ¦ßÆ"};
const unsigned char PY_mb_zai[] = {"ÔÚÔÙÔØÔÔÔÖ×ĞÔÕÔ×áÌçŞ"};
const unsigned char PY_mb_zan[] = {"ÔŞÔÛÔİÔÜŞÙè¶êÃô¢ôØôõöÉ"};
const unsigned char PY_mb_zang[] = {"²ØÔàÔáÔßŞÊæàê°"};
const unsigned char PY_mb_zao[] = {"ÔìÔçÔâÔïÔäÔãÔæÔíÔåÔèÔéÔêÔëÔîßğ"};
const unsigned char PY_mb_ze[] = {"ÔòÔñÔğ²àÔóÕ¦ØÆØÓßõàıåÅê¾óĞóåô·"};
const unsigned char PY_mb_zei[] = {"Ôô"};
const unsigned char PY_mb_zen[] = {"ÔõÚÚ"};
const unsigned char PY_mb_zeng[] = {"ÔöÔø×ÛÔ÷ÔùçÕêµîÀï­"};
const unsigned char PY_mb_zh[] = {"ÔúÕ¨ÕªÕ®Õ½Õ¹³¤ÕÅÕÂÖÓ×ÅÕÕÕÒÕâÕæÕùÕıÕşÖ®ÖÆÖÎÖ»ÖĞÖÜÖİÖ÷×¡×¥×ª×¨×«×¬×°×´×¼"};
const unsigned char PY_mb_zha[] = {"²éÔúÕ¨Õ¢Õ¡ÔşÔüÀ¯ÔûÔıÕ£Õ¤Õ¥Õ¦Õ§Õ©×õŞêß¸ßåßîà©é«íÄğäòÆ÷ş"};
const unsigned char PY_mb_zhai[] = {"Õ¯²àÔñÕªÕ­Õ«µÔ¼ÀÕ¬Õ®íÎñ©"};
const unsigned char PY_mb_zhan[] = {"Õ½Õ¹Õ¾Õ¼Õ³²üÕ°Õ±Õ²Õ´ÕµÕ¶Õ·Õ¸ÕºÕ»Õ¿ÕÀÚŞŞøì¹"};
const unsigned char PY_mb_zhang[] = {"³¤ÕÅÕÂÕÆÕÌÕÏÕÍÕÇÕËÕÁÕÃÕÄÕÈÕÉÕÊÕÎØëÛµá¤áÖâ¯æÑè°ó¯"};
const unsigned char PY_mb_zhao[] = {"×ÅÕÕÕÒÕĞ³¯ÕÙÕÔ×¦ÕÖÕÓÕ×³°ÕÑÕØÚ¯ßúèşîÈóÉ"};
const unsigned char PY_mb_zhe[] = {"Õâ×ÅÕßÕÛÕÜÕãÕÚÕİÕŞÕàÕáÚØß¡èÏéüíİğÑñŞòØô÷"};
const unsigned char PY_mb_zhen[] = {"ÕæÕëÕóÕòÕñÕğÕäÕïÕåÕçÕèÕéÕêÕìÕíÕîÛÚİèä¥äÚçÇèåé©é»éôêâëÓëŞìõî³ğ¡ğ²óğ"};
const unsigned char PY_mb_zheng[] = {"ÕùÕıÕşÕûÖ¤Õ÷ÕôÖ¢¶¡Ö£ÕõÕöÕøÕúÕüÖ¡Úºá¿áçîÛï£óİ"};
const unsigned char PY_mb_zhi[] = {"Ö®ÖÆÖÎÖ»ÖÊÖ¸Ö±Ö§Ö¯Ö¹ÖÁÖÃÖ¾ÖµÖªÊ¶Ö´Ö°Ö²Ö½ÖÂÖ¦Ö³Ö¬ÖÇÊÏÖ«ÖÈÖ·ÖÍÖ­Ö¥Ö¨Ö©Ö¶ÖºÖ¼Ö¿ÖÀÖÄÖÅÖÉÖËÖÌÖÏØ´ÚìÛ¤ÛúÜÆŞıàùáçâååéåëæïèÎè×èÙèäéòéùêŞëÕëùìíìóíéïôğºğëòÎôêõ¥õÅõÙõÜõôö£"};
const unsigned char PY_mb_zhong[] = {"ÖĞÖÖÖØÖÚÖÓÖÕÖÒÖ×ÖÙÖÑÖÔÚ£âìïñó®ô±õà"};
const unsigned char PY_mb_zhou[] = {"ÖáÖÜÖŞÖİÖåÖèÖÛÖßÖàÖâÖãÖäÖæÖçİ§ßúæ¨æûç§ëĞíØô¦ôíôü"};
const unsigned char PY_mb_zhu[] = {"Ö÷×¢Öø×¡ÖúÖíÖìÖıÊôÖêÖşÖùÊõ×¤Öğ×£ÖñÖüÖéÖîÖëÖïÖòÖóÖôÖõÖöÖûØùÙªÛ¥Üïä¨ä¾äóèÌéÆéÍìÄîùğæğññÒóÃóçô¶ôãõî÷æ"};
const unsigned char PY_mb_zhua[] = {"×¥×¦ÎÎ"};
const unsigned char PY_mb_zhuai[] = {"×ªÒ·×§àÜ"};
const unsigned char PY_mb_zhuan[] = {"×ª×¨´«×©×«×¬×­ßùâÍãçò§"};
const unsigned char PY_mb_zhuang[] = {"×°×´×³×¯×²´±×®×±Ù×ŞÊ"};
const unsigned char PY_mb_zhui[] = {"×·×¶×µ×¸×¹×ºã·æíçÄ"};
const unsigned char PY_mb_zhun[] = {"×¼ÍÍ×»ëÆñ¸"};
const unsigned char PY_mb_zhuo[] = {"×ÅÖø×½×À½É×¾×¿×Á×Â×Ã×Ä×Æ×ÇÙ¾ÚÂßªä·äÃåªèşìÌìúí½ïí"};
const unsigned char PY_mb_zi[] = {"×Ó×Ô×Ê×Ö×Ï×Ğ×Ñ×ËÖ¨×È×É×Ì×Í×Î×Ò×ÕÚÑÜëßÚáÑæ¢æÜç»è÷ê¢êßí§íöïÅïöñèóÊôÒôôõşö¤ö·öö÷Ú"};
const unsigned char PY_mb_zong[] = {"×Ü×İ×Ú×Û×Ø×××ÙÙÌèÈëêôÕ"};
const unsigned char PY_mb_zou[] = {"×ß×Ş×à×áÚÁÚîÛ¸æãöí"};
const unsigned char PY_mb_zu[] = {"×é×å×ã×è×æ×â×ä×çÙŞİÏïß"};
const unsigned char PY_mb_zuan[] = {"×ê×¬×ëß¬çÚõò"};
const unsigned char PY_mb_zui[] = {"×î×ï×ì¶Ñ¾××íŞ©"};
const unsigned char PY_mb_zun[] = {"×ğ×ñß¤é×÷®"};
const unsigned char PY_mb_zuo[] = {"×÷×ö×ó×ù×ø×ò´éÔä´ê×Á×ô×õÚèßòàÜâôëÑìñóĞ"};

const T9PY_IDX t9PY_index[] = {
    {"2", "a", PY_mb_a},
    {"2", "b", PY_mb_b},
    {"2", "c", PY_mb_c},
    {"3", "d", PY_mb_d},
    {"3", "e", PY_mb_e},
    {"3", "f", PY_mb_f},
    {"4", "g", PY_mb_g},
    {"4", "h", PY_mb_h},
    {"5", "j", PY_mb_j},
    {"5", "k", PY_mb_k},
    {"5", "l", PY_mb_l},
    {"6", "m", PY_mb_m},
    {"6", "n", PY_mb_n},
    {"6", "o", PY_mb_o},
    {"7", "p", PY_mb_p},
    {"7", "q", PY_mb_q},
    {"7", "r", PY_mb_r},
    {"7", "s", PY_mb_s},
    {"8", "t", PY_mb_t},
    {"9", "w", PY_mb_w},
    {"9", "x", PY_mb_x},
    {"9", "y", PY_mb_y},
    {"9", "z", PY_mb_z},
    {"22", "ba", PY_mb_ba},
    {"22", "ca", PY_mb_ca},
    {"23", "ce", PY_mb_ce},
    {"23", "be", PY_mb_bei},
    {"24", "ai", PY_mb_ai},
    {"24", "bi", PY_mb_bi},
    {"24", "ch", PY_mb_ch},
    {"24", "ci", PY_mb_ci},
    {"26", "co", PY_mb_cou},
    {"26", "bo", PY_mb_bo},
    {"26", "an", PY_mb_an},
    {"26", "ao", PY_mb_ao},
    {"28", "bu", PY_mb_bu},
    {"28", "cu", PY_mb_cu},
    {"32", "da", PY_mb_da},
    {"32", "fa", PY_mb_fa},
    {"33", "de", PY_mb_de},
    {"33", "fe", PY_mb_fei},
    {"34", "di", PY_mb_di},
    {"34", "ei", PY_mb_ei},
    {"36", "en", PY_mb_en},
    {"36", "fo", PY_mb_fo},
    {"37", "er", PY_mb_er},
    {"36", "do", PY_mb_dou},
    {"38", "du", PY_mb_du},
    {"38", "fu", PY_mb_fu},
    {"42", "ga", PY_mb_ga},
    {"42", "ha", PY_mb_ha},
    {"43", "ge", PY_mb_ge},
    {"43", "he", PY_mb_he},
    {"46", "go", PY_mb_gou},
    {"46", "ho", PY_mb_hou},
    {"48", "gu", PY_mb_gu},
    {"48", "hu", PY_mb_hu},
    {"52", "ka", PY_mb_ka},
    {"52", "la", PY_mb_la},
    {"53", "le", PY_mb_le},
    {"53", "ke", PY_mb_ke},
    {"54", "ji", PY_mb_ji},
    {"54", "li", PY_mb_li},
    {"56", "ko", PY_mb_kou},
    {"56", "lo", PY_mb_lo},
    {"58", "lu", PY_mb_lu},
    {"58", "lv", PY_mb_lv},
    {"58", "ju", PY_mb_ju},
    {"58", "ku", PY_mb_ku},
    {"62", "ma", PY_mb_ma},
    {"62", "na", PY_mb_na},
    {"63", "me", PY_mb_me},
    {"63", "ne", PY_mb_ne},
    {"64", "mi", PY_mb_mi},
    {"64", "ni", PY_mb_ni},
    {"66", "mo", PY_mb_mo},
    {"66", "no", PY_mb_nou},
    {"68", "mu", PY_mb_mu},
    {"68", "nu", PY_mb_nu},
    {"68", "nv", PY_mb_nv},
    {"68", "ou", PY_mb_ou},
    {"72", "pa", PY_mb_pa},
    {"72", "pe", PY_mb_pei},
    {"72", "ra", PY_mb_ran},
    {"72", "sa", PY_mb_sa},
    {"73", "re", PY_mb_re},
    {"73", "se", PY_mb_se},
    {"74", "pi", PY_mb_pi},
    {"74", "qi", PY_mb_qi},
    {"74", "ri", PY_mb_ri},
    {"74", "sh", PY_mb_sh},
    {"74", "si", PY_mb_si},
    {"76", "so", PY_mb_sou},
    {"76", "po", PY_mb_po},
    {"76", "ro", PY_mb_rou},
    {"78", "pu", PY_mb_pu},
    {"78", "qu", PY_mb_qu},
    {"78", "ru", PY_mb_ru},
    {"78", "su", PY_mb_su},
    {"82", "ta", PY_mb_ta},
    {"83", "te", PY_mb_te},
    {"84", "ti", PY_mb_ti},
    {"86", "to", PY_mb_tou},
    {"88", "tu", PY_mb_tu},
    {"92", "wa", PY_mb_wa},
    {"92", "ya", PY_mb_ya},
    {"92", "za", PY_mb_za},
    {"93", "ze", PY_mb_ze},
    {"93", "ye", PY_mb_ye},
    {"94", "yi", PY_mb_yi},
    {"94", "xi", PY_mb_xi},
    {"94", "zh", PY_mb_zh},
    {"94", "zi", PY_mb_zi},
    {"96", "zo", PY_mb_zou},
    {"96", "wo", PY_mb_wo},
    {"96", "yo", PY_mb_yo},
    {"98", "wu", PY_mb_wu},
    {"98", "xu", PY_mb_xu},
    {"98", "yu", PY_mb_yu},
    {"98", "zu", PY_mb_zu},
    {"224", "bai", PY_mb_bai},
    {"224", "cai", PY_mb_cai},
    {"226", "can", PY_mb_can},
    {"226", "cao", PY_mb_cao},
    {"226", "ban", PY_mb_ban},
    {"226", "bao", PY_mb_bao},
    {"234", "bei", PY_mb_bei},
    {"236", "ben", PY_mb_ben},
    {"236", "cen", PY_mb_cen},
    {"242", "bia", PY_mb_bian},
    {"242", "cha", PY_mb_cha},
    {"243", "che", PY_mb_che},
    {"243", "bie", PY_mb_bie},
    {"244", "chi", PY_mb_chi},
    {"246", "bin", PY_mb_bin},
    {"246", "cho", PY_mb_chou},
    {"248", "chu", PY_mb_chu},
    {"264", "ang", PY_mb_ang},
    {"266", "con", PY_mb_cong},
    {"268", "cou", PY_mb_cou},
    {"282", "cua", PY_mb_cuan},
    {"284", "cui", PY_mb_cui},
    {"286", "cun", PY_mb_cun},
    {"286", "cuo", PY_mb_cuo},
    {"324", "dai", PY_mb_dai},
    {"326", "dan", PY_mb_dan},
    {"326", "dao", PY_mb_dao},
    {"326", "fan", PY_mb_fan},
    {"334", "fei", PY_mb_fei},
    {"336", "fen", PY_mb_fen},
    {"336", "den", PY_mb_deng},
    {"342", "dia", PY_mb_dia},
    {"343", "die", PY_mb_die},
    {"346", "din", PY_mb_ding},
    {"348", "diu", PY_mb_diu},
    {"366", "don", PY_mb_dong},
    {"368", "dou", PY_mb_dou},
    {"368", "fou", PY_mb_fou},
    {"382", "dua", PY_mb_duan},
    {"384", "dui", PY_mb_dui},
    {"386", "dun", PY_mb_dun},
    {"386", "duo", PY_mb_duo},
    {"424", "gai", PY_mb_gai},
    {"424", "hai", PY_mb_hai},
    {"426", "han", PY_mb_han},
    {"426", "gan", PY_mb_gan},
    {"426", "gao", PY_mb_gao},
    {"426", "hao", PY_mb_hao},
    {"434", "hei", PY_mb_hei},
    {"434", "gei", PY_mb_gei},
    {"436", "gen", PY_mb_gen},
    {"436", "hen", PY_mb_hen},
    {"466", "gon", PY_mb_gong},
    {"466", "hon", PY_mb_hong},
    {"468", "gou", PY_mb_gou},
    {"468", "hou", PY_mb_hou},
    {"482", "hua", PY_mb_hua},
    {"482", "gua", PY_mb_gua},
    {"484", "gui", PY_mb_gui},
    {"484", "hui", PY_mb_hui},
    {"486", "hun", PY_mb_hun},
    {"486", "huo", PY_mb_huo},
    {"486", "gun", PY_mb_gun},
    {"486", "guo", PY_mb_guo},
    {"524", "kai", PY_mb_kai},
    {"524", "lai", PY_mb_lai},
    {"526", "lan", PY_mb_lan},
    {"526", "kan", PY_mb_kan},
    {"526", "kao", PY_mb_kao},
    {"526", "lao", PY_mb_lao},
    {"534", "lei", PY_mb_lei},
    {"536", "len", PY_mb_leng},
    {"536", "ken", PY_mb_ken},
    {"542", "jia", PY_mb_jia},
    {"542", "lia", PY_mb_lia},
    {"543", "jie", PY_mb_jie},
    {"543", "lie", PY_mb_lie},
    {"546", "lin", PY_mb_lin},
    {"546", "jin", PY_mb_jin},
    {"546", "jio", PY_mb_jiong},
    {"548", "jiu", PY_mb_jiu},
    {"548", "liu", PY_mb_liu},
    {"566", "lon", PY_mb_long},
    {"566", "kon", PY_mb_kong},
    {"568", "kou", PY_mb_kou},
    {"568", "lou", PY_mb_lou},
    {"582", "lua", PY_mb_luan},
    {"582", "kua", PY_mb_kua},
    {"582", "jua", PY_mb_juan},
    {"583", "jue", PY_mb_jue},
    {"583", "lue", PY_mb_lue},
    {"584", "kui", PY_mb_kui},
    {"586", "lun", PY_mb_lun},
    {"586", "luo", PY_mb_luo},
    {"586", "kun", PY_mb_kun},
    {"586", "kuo", PY_mb_kuo},
    {"586", "jun", PY_mb_jun},
    {"624", "mai", PY_mb_mai},
    {"624", "nai", PY_mb_nai},
    {"626", "nan", PY_mb_nan},
    {"626", "man", PY_mb_man},
    {"626", "mao", PY_mb_mao},
    {"626", "nao", PY_mb_nao},
    {"634", "nei", PY_mb_nei},
    {"634", "mei", PY_mb_mei},
    {"636", "nen", PY_mb_nen},
    {"636", "men", PY_mb_men},
    {"642", "mia", PY_mb_mian},
    {"642", "nia", PY_mb_nian},
    {"643", "mie", PY_mb_mie},
    {"643", "nie", PY_mb_nie},
    {"646", "nin", PY_mb_nin},
    {"646", "min", PY_mb_min},
    {"648", "miu", PY_mb_miu},
    {"648", "niu", PY_mb_niu},
    {"666", "non", PY_mb_nong},
    {"668", "mou", PY_mb_mou},
    {"668", "nou", PY_mb_nou},
    {"682", "nua", PY_mb_nuan},
    {"683", "nve", PY_mb_nve},
    {"686", "nuo", PY_mb_nuo},
    {"724", "pai", PY_mb_pai},
    {"734", "pei", PY_mb_pei},
    {"724", "sai", PY_mb_sai},
    {"726", "san", PY_mb_san},
    {"726", "pan", PY_mb_pan},
    {"726", "pao", PY_mb_pao},
    {"736", "pen", PY_mb_pen},
    {"726", "ran", PY_mb_ran},
    {"726", "rao", PY_mb_rao},
    {"726", "sao", PY_mb_sao},
    {"736", "sen", PY_mb_sen},
    {"736", "ren", PY_mb_ren},
    {"742", "pia", PY_mb_pian},
    {"742", "qia", PY_mb_qia},
    {"742", "sha", PY_mb_sha},
    {"743", "pie", PY_mb_pie},
    {"743", "qie", PY_mb_qie},
    {"743", "she", PY_mb_she},
    {"744", "shi", PY_mb_shi},
    {"748", "shu", PY_mb_shu},
    {"746", "qin", PY_mb_qin},
    {"746", "pin", PY_mb_pin},
    {"746", "qio", PY_mb_qiong},
    {"746", "sho", PY_mb_shou},
    {"748", "qiu", PY_mb_qiu},
    {"766", "ron", PY_mb_rong},
    {"768", "pou", PY_mb_pou},
    {"768", "rou", PY_mb_rou},
    {"766", "son", PY_mb_song},
    {"768", "sou", PY_mb_sou},
    {"782", "sua", PY_mb_suan},
    {"782", "rua", PY_mb_ruan},
    {"782", "qua", PY_mb_quan},
    {"783", "que", PY_mb_que},
    {"784", "rui", PY_mb_rui},
    {"784", "sui", PY_mb_sui},
    {"786", "sun", PY_mb_sun},
    {"786", "suo", PY_mb_suo},
    {"786", "run", PY_mb_run},
    {"786", "ruo", PY_mb_ruo},
    {"786", "qun", PY_mb_qun},
    {"824", "tai", PY_mb_tai},
    {"826", "tan", PY_mb_tan},
    {"826", "tao", PY_mb_tao},
    {"836", "ten", PY_mb_teng},
    {"842", "tia", PY_mb_tian},
    {"843", "tie", PY_mb_tie},
    {"846", "tin", PY_mb_ting},
    {"866", "ton", PY_mb_tong},
    {"868", "tou", PY_mb_tou},
    {"882", "tua", PY_mb_tuan},
    {"884", "tui", PY_mb_tui},
    {"886", "tun", PY_mb_tun},
    {"886", "tuo", PY_mb_tuo},
    {"924", "wai", PY_mb_wai},
    {"924", "zai", PY_mb_zai},
    {"926", "zan", PY_mb_zan},
    {"926", "wan", PY_mb_wan},
    {"926", "yan", PY_mb_yan},
    {"926", "yao", PY_mb_yao},
    {"926", "zao", PY_mb_zao},
    {"934", "zei", PY_mb_zei},
    {"934", "wei", PY_mb_wei},
    {"936", "zen", PY_mb_zen},
    {"936", "wen", PY_mb_wen},
    {"942", "xia", PY_mb_xia},
    {"942", "zha", PY_mb_zha},
    {"943", "xie", PY_mb_xie},
    {"943", "zhe", PY_mb_zhe},
    {"944", "zhi", PY_mb_zhi},
    {"946", "zho", PY_mb_zhou},
    {"946", "xin", PY_mb_xin},
    {"946", "xio", PY_mb_xiong},
    {"946", "yin", PY_mb_yin},
    {"948", "xiu", PY_mb_xiu},
    {"948", "zhu", PY_mb_zhu},
    {"966", "yon", PY_mb_yong},
    {"966", "zon", PY_mb_zong},
    {"968", "you", PY_mb_you},
    {"968", "zou", PY_mb_zou},
    {"982", "zua", PY_mb_zuan},
    {"982", "yua", PY_mb_yuan},
    {"982", "xua", PY_mb_xuan},
    {"983", "xue", PY_mb_xue},
    {"983", "yue", PY_mb_yue},
    {"984", "zui", PY_mb_zui},
    {"986", "zun", PY_mb_zun},
    {"986", "zuo", PY_mb_zuo},
    {"986", "yun", PY_mb_yun},
    {"986", "xun", PY_mb_xun},
    {"2264", "bang", PY_mb_bang},
    {"2364", "beng", PY_mb_beng},
    {"2426", "bian", PY_mb_bian},
    {"2426", "biao", PY_mb_biao},
    {"2464", "bing", PY_mb_bing},
    {"2264", "cang", PY_mb_cang},
    {"2364", "ceng", PY_mb_ceng},
    {"2424", "chai", PY_mb_chai},
    {"2426", "chan", PY_mb_chan},
    {"2426", "chao", PY_mb_chao},
    {"2436", "chen", PY_mb_chen},
    {"2466", "chon", PY_mb_chong},
    {"2468", "chou", PY_mb_chou},
    {"2482", "chua", PY_mb_chuai},
    {"2484", "chui", PY_mb_chui},
    {"2486", "chun", PY_mb_chun},
    {"2486", "chuo", PY_mb_chuo},
    {"2664", "cong", PY_mb_cong},
    {"2826", "cuan", PY_mb_cuan},
    {"3264", "dang", PY_mb_dang},
    {"3264", "fang", PY_mb_fang},
    {"3364", "feng", PY_mb_feng},
    {"3364", "deng", PY_mb_deng},
    {"3426", "dian", PY_mb_dian},
    {"3426", "diao", PY_mb_diao},
    {"3464", "ding", PY_mb_ding},
    {"3664", "dong", PY_mb_dong},
    {"3826", "duan", PY_mb_duan},
    {"4264", "gang", PY_mb_gang},
    {"4364", "geng", PY_mb_geng},
    {"4664", "gong", PY_mb_gong},
    {"4824", "guai", PY_mb_guai},
    {"4826", "guan", PY_mb_guan},
    {"4264", "hang", PY_mb_hang},
    {"4364", "heng", PY_mb_heng},
    {"4664", "hong", PY_mb_hong},
    {"4824", "huai", PY_mb_huai},
    {"4826", "huan", PY_mb_huan},
    {"5264", "kang", PY_mb_kang},
    {"5264", "lang", PY_mb_lang},
    {"5364", "leng", PY_mb_leng},
    {"5364", "keng", PY_mb_keng},
    {"5426", "lian", PY_mb_lian},
    {"5426", "liao", PY_mb_liao},
    {"5426", "jian", PY_mb_jian},
    {"5426", "jiao", PY_mb_jiao},
    {"5464", "jing", PY_mb_jing},
    {"5464", "ling", PY_mb_ling},
    {"5466", "jion", PY_mb_jiong},
    {"5664", "long", PY_mb_long},
    {"5664", "kong", PY_mb_kong},
    {"5824", "kuai", PY_mb_kuai},
    {"5826", "kuan", PY_mb_kuan},
    {"5826", "luan", PY_mb_luan},
    {"5826", "juan", PY_mb_juan},
    {"6264", "mang", PY_mb_mang},
    {"6264", "nang", PY_mb_nang},
    {"6364", "meng", PY_mb_meng},
    {"6364", "neng", PY_mb_neng},
    {"6426", "mian", PY_mb_mian},
    {"6426", "miao", PY_mb_miao},
    {"6426", "nian", PY_mb_nian},
    {"6426", "niao", PY_mb_niao},
    {"6464", "ming", PY_mb_ming},
    {"6464", "ning", PY_mb_ning},
    {"6664", "nong", PY_mb_nong},
    {"6826", "nuan", PY_mb_nuan},
    {"7264", "pang", PY_mb_pang},
    {"7364", "peng", PY_mb_peng},
    {"7264", "rang", PY_mb_rang},
    {"7264", "sang", PY_mb_sang},
    {"7364", "seng", PY_mb_seng},
    {"7364", "reng", PY_mb_reng},
    {"7424", "shai", PY_mb_shai},
    {"7426", "shan", PY_mb_shan},
    {"7426", "shao", PY_mb_shao},
    {"7426", "pian", PY_mb_pian},
    {"7426", "piao", PY_mb_piao},
    {"7426", "qian", PY_mb_qian},
    {"7426", "qiao", PY_mb_qiao},
    {"7436", "shen", PY_mb_shen},
    {"7464", "ping", PY_mb_ping},
    {"7464", "qing", PY_mb_qing},
    {"7466", "qion", PY_mb_qiong},
    {"7468", "shou", PY_mb_shou},
    {"7482", "shua", PY_mb_shua},
    {"7484", "shui", PY_mb_shui},
    {"7486", "shun", PY_mb_shun},
    {"7486", "shuo", PY_mb_shuo},
    {"7664", "rong", PY_mb_rong},
    {"7664", "song", PY_mb_song},
    {"7826", "suan", PY_mb_suan},
    {"7826", "quan", PY_mb_quan},
    {"7826", "ruan", PY_mb_ruan},
    {"8264", "tang", PY_mb_tang},
    {"8364", "teng", PY_mb_teng},
    {"8426", "tian", PY_mb_tian},
    {"8426", "tiao", PY_mb_tiao},
    {"8464", "ting", PY_mb_ting},
    {"8664", "tong", PY_mb_tong},
    {"8826", "tuan", PY_mb_tuan},
    {"9264", "wang", PY_mb_wang},
    {"9264", "zang", PY_mb_zang},
    {"9264", "yang", PY_mb_yang},
    {"9364", "weng", PY_mb_weng},
    {"9364", "zeng", PY_mb_zeng},
    {"9424", "zhai", PY_mb_zhai},
    {"9426", "zhan", PY_mb_zhan},
    {"9426", "zhao", PY_mb_zhao},
    {"9426", "xian", PY_mb_xian},
    {"9426", "xiao", PY_mb_xiao},
    {"9436", "zhen", PY_mb_zhen},
    {"9464", "xing", PY_mb_xing},
    {"9464", "ying", PY_mb_ying},
    {"9466", "zhon", PY_mb_zhong},
    {"9468", "zhou", PY_mb_zhou},
    {"9482", "zhua", PY_mb_zhua},
    {"9484", "zhui", PY_mb_zhui},
    {"9486", "zhun", PY_mb_zhun},
    {"9486", "zhuo", PY_mb_zhuo},
    {"9466", "xion", PY_mb_xiong},
    {"9664", "yong", PY_mb_yong},
    {"9664", "zong", PY_mb_zong},
    {"9826", "yuan", PY_mb_yuan},
    {"9826", "xuan", PY_mb_xuan},
    {"9826", "zuan", PY_mb_zuan},
    {"94264", "xiang", PY_mb_xiang},
    {"94264", "zhang", PY_mb_zhang},
    {"94364", "zheng", PY_mb_zheng},
    {"94664", "xiong", PY_mb_xiong},
    {"94664", "zhong", PY_mb_zhong},
    {"94824", "zhuai", PY_mb_zhuai},
    {"94826", "zhuan", PY_mb_zhuan},
    {"24264", "chang", PY_mb_chang},
    {"24364", "cheng", PY_mb_cheng},
    {"24664", "chong", PY_mb_chong},
    {"24824", "chuai", PY_mb_chuai},
    {"24826", "chuan", PY_mb_chuan},
    {"48264", "guang", PY_mb_guang},
    {"48264", "huang", PY_mb_huang},
    {"54264", "jiang", PY_mb_jiang},
    {"54264", "liang", PY_mb_liang},
    {"54664", "jiong", PY_mb_jiong},
    {"58264", "kuang", PY_mb_kuang},
    {"64264", "niang", PY_mb_niang},
    {"74264", "qiang", PY_mb_qiang},
    {"74264", "shang", PY_mb_shang},
    {"74364", "sheng", PY_mb_sheng},
    {"74664", "qiong", PY_mb_qiong},
    {"74824", "shuai", PY_mb_shuai},
    {"74826", "shuan", PY_mb_shuan},
    {"248264", "chuang", PY_mb_chuang},
    {"748264", "shuang", PY_mb_shuang},
    {"948264", "zhuang", PY_mb_zhuang},
    {"", "", PY_mb_space}};

static void DataFormat2(char *Buf, char *str, long data1, long data2);

// ±È½ÏÁ½¸ö×Ö·û´®µÄÆ¥ÅäÇé¿ö
//·µ»ØÖµ:0xff,±íÊ¾ÍêÈ«Æ¥Åä.
//        ÆäËû,Æ¥ÅäµÄ×Ö·ûÊı
u8 str_match(u8 *str1, u8 *str2)
{
    u8 i = 0;
    while (1)
    {
        if (*str1 != *str2)
            break; //²¿·ÖÆ¥Åä
        if (*str1 == '\0')
        {
            i = 0XFF;
            break;
        } //ÍêÈ«Æ¥Åä
        i++;
        str1++;
        str2++;
    }
    return i; //Á½¸ö×Ö·û´®ÏàµÈ
}

//»ñÈ¡Æ¥ÅäµÄÆ´ÒôÂë±í
//*strin,ÊäÈëµÄ×Ö·û´®,ĞÎÈç:"726"
//**matchlist,Êä³öµÄÆ¥Åä±í.
//·µ»ØÖµ:[7],0,±íÊ¾ÍêÈ«Æ¥Åä£»1£¬±íÊ¾²¿·ÖÆ¥Åä£¨½öÔÚÃ»ÓĞÍêÈ«Æ¥ÅäµÄÊ±ºò²Å»á³öÏÖ£©
// [6:0],ÍêÈ«Æ¥ÅäµÄÊ±ºò£¬±íÊ¾ÍêÈ«Æ¥ÅäµÄÆ´Òô¸öÊı
//            ²¿·ÖÆ¥ÅäµÄÊ±ºò£¬±íÊ¾ÓĞĞ§Æ¥ÅäµÄÎ»Êı
u8 get_matched_pymb(u8 *strin, T9PY_IDX **matchlist)
{
    T9PY_IDX *bestmatch; //×î¼ÑÆ¥Åä
    u16 pyindex_len;
    u16 i;
    u8 temp, mcnt = 0, bmcnt = 0;
    bestmatch = (T9PY_IDX *)&t9PY_index[0];                   //Ä¬ÈÏÎªa µÄÆ¥Åä
    pyindex_len = sizeof(t9PY_index) / sizeof(t9PY_index[0]); //µÃµ½py Ë÷Òı±íµÄ´óĞ¡.
    for (i = 0; i < pyindex_len; i++)
    {
        temp = str_match(strin, (u8 *)t9PY_index[i].PY);
        if (temp)
        {
            if (temp == 0XFF)
            {
                matchlist[mcnt++] = (T9PY_IDX *)&t9PY_index[i];
            }
            else if (temp > bmcnt) //ÕÒ×î¼ÑÆ¥Åä
            {
                bmcnt = temp;
                bestmatch = (T9PY_IDX *)&t9PY_index[i]; //×îºÃµÄÆ¥Åä.
            }
        }
    }
    if (mcnt == 0 && bmcnt) //Ã»ÓĞÍêÈ«Æ¥ÅäµÄ½á¹û,µ«ÊÇÓĞ²¿·ÖÆ¥ÅäµÄ½á¹û
    {
        matchlist[0] = bestmatch;
        mcnt = bmcnt | 0X80; //·µ»Ø²¿·ÖÆ¥ÅäµÄÓĞĞ§Î»Êı
    }
    return mcnt; //·µ»ØÆ¥ÅäµÄ¸öÊı
}

//µÃµ½Æ´ÒôÂë±í.
//str:ÊäÈë×Ö·û´®
//·µ»ØÖµ:Æ¥Åä¸öÊı.
u8 get_pymb(u8 *str, u8 *mb)
{
    u8 mcnt = get_matched_pymb(str, t9);
    u8 i, l, mbl;
    l = 0;
    for (i = 0; i < (mcnt & 0x7F); i++)
    {
        mbl = strlen((const char *)(t9[i]->MB));
        if ((l + mbl) > 126) //»º´æÇøÓò128×Ö½Ú
        {
            mbl = 126 - l;
        }
        memcpy(mb, t9[i]->MB, mbl);
        mb += mbl;
        l += mbl;
        if (l >= 126)
        {
            break;
        }
    }
    return l;
}

//unsigned char readbyte(unsigned long adrr);

unsigned char hzdispwin = 7; //Ã¿ÆÁºº×ÖÏÔÊ¾¸öÊı

typedef struct
{
    unsigned char py[6];  //Æ´Òô (6BYTE)
    unsigned short begin; //¶ÔÓ¦µÄÂë±íÆğÊ¼ĞòºÅ (2BYTE)
} PYCODETABLE;            // (8BYTE)

int pyfd;                         //Æ´ÒôÂë±í¾ä±ú
unsigned short pysum;             //Æ´Òô×éºÏµÄ¸öÊı
unsigned short hzsum;             //Âë±íÖĞµÄºº×Ö¸öÊı
unsigned short pyidx[26];         //ÒÔ26 ¸öÓ¢ÎÄ×ÖÄ¸¿ªÍ·µÄÆ´Òô´®Ë÷Òı
unsigned short pyseek;            //Æ´Òô²éÕÒµÄ¿ªÊ¼Î»ÖÃ ,ÒÆ¶¯Ö¸ÕëÓÃ
unsigned short firsthz;           //µÚÒ»¸öºº×ÖËùÔÚÎ»ÖÃ£¬ÎªÁËÓÃhzseek ¼ÆËã×îºóÒ»¸öÆ´ÒôµÄºº×Ö¸öÊı
unsigned short hzseek;            //ºº×Ö²éÕÒµÄ¿ªÊ¼Î»ÖÃ,curhz ÒÆ¶¯Ö¸ÕëÓÃ
unsigned short curhz = 0;         //µ±Ç°ÏÔÊ¾ºº×ÖµÄºÅ£¬ÉÏÏÂÒÆ¶¯¾àÀëÎªhzdispwin
unsigned short maxcurhz;          //µ±Ç°Æ´ÒôµÄ×î´óºº×ÖÊı
unsigned char hzstring[20];       //±£´æºº×ÖÄÚÂë£¬´óĞ¡Ó¦Îªhzdispwin*2+1//ºº×ÖÏÔÊ¾¸öÊıMAX=10 ¸ö
unsigned long baseaddr = 0x72790; //¸ù¾İ×Ö¿âĞ¾Æ¬µÄÊµ¼ÊµØÖ·½øĞĞĞŞ¸Ä£¨¼ûÊı¾İÊÖ²á£©

void pyInit(void)
{
    short i;
    unsigned long addr = 0x0000;
    PYCODETABLE py;
    unsigned char bytmp;
    addr = 0x0000 + baseaddr; //pysum Æ´Òô×éºÏÊı(2BYTE)
    //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);
    pysum = bytmp;
    pysum = pysum << 8;
    //SpiFlash_Read(&bytmp,addr,1);
    read_font_addr(addr, &bytmp, 1);

    pysum = pysum + bytmp;
    addr = 0x0002 + baseaddr; //hzsum ¶ÁÈëºº×Ö×éºÏÊı(2BYTE)
                              //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);

    hzsum = bytmp;
    hzsum = hzsum << 8;
    //SpiFlash_Read(&bytmp,addr,1);
    read_font_addr(addr, &bytmp, 1);

    hzsum = hzsum + bytmp;
    addr = 0x0004 + baseaddr; //ÒÔ26 ¸öÓ¢ÎÄ×ÖÄ¸¿ªÍ·µÄÆ´Òô´®Ë÷Òı(26*2=52BYTE)
    for (i = 0; i < 26; i++)
    {
        //SpiFlash_Read(&bytmp,addr++,1);
        read_font_addr(addr++, &bytmp, 1);

        pyidx[i] = bytmp;
        pyidx[i] = pyidx[i] << 8;
        //SpiFlash_Read(&bytmp,addr++,1);
        read_font_addr(addr++, &bytmp, 1);

        pyidx[i] = pyidx[i] + bytmp;
    }

    pyseek = pyidx[0];        //Æ´Òô²éÕÒµÄÏà¶Ô¿ªÊ¼µØÖ· ,ÒÆ¶¯Ö¸ÕëÓÃ
    addr = pyseek + baseaddr; //³õÊ¼»¯Ê±Ë÷Òıµ½µÚÒ»×ÖÄ¸¡°a¡±µØÖ·(³õÊ¼Öµ)
    for (i = 0; i < 6; i++)
    {
        //SpiFlash_Read(&py.py[i],addr++,1);
        read_font_addr(addr++, &py.py[i], 1);
    }
    //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);

    py.begin = bytmp;
    py.begin = py.begin << 8;
    //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);

    py.begin = py.begin + bytmp;
    firsthz = py.begin;
    //for(i = 0; i < 60000;i++)
    //{
    //	addr = baseaddr+(i*20);
    //	SpiFlash_Read(hzstring,addr,20);
    //}
}

void Pinyin_Init(void)
{
    Pinyin.Enable = false;
    Pinyin.SelPage = 0;
    Pinyin.SelIndex = -1;
    Pinyin.UpdateFlag = 0;
    memset(Pinyin.InputBuf, 0, sizeof(Pinyin.InputBuf));
    memset(Pinyin.DataBuf, 0, sizeof(Pinyin.DataBuf));
    memset(Pinyin.SelData, 0, sizeof(Pinyin.SelData));
    pyInit();
}

u16 Pinyin_GetStringData(void)
{
    u8 HIndex = 0;
    //	uint32_t  Pinyin_Loc=0;
    //	u8 i=0,k=0;
    u8 Len = strlen(Pinyin.InputBuf);
    if (Len == 0)
        return 0;
#if 0
	strlower(Pinyin.InputBuf);
	for(i=0;i<8;i++)
	{
		if((Pinyin.InputBuf[i]>='a')&&(Pinyin.InputBuf[i]<='z'))
		{
			u8 buffer[8];
			u8 j=0;
			HIndex = Pinyin.InputBuf[i]- 'a';
			if(PinyinHeaderIndex[HIndex]==0xFFFF)
				return 0;
			Pinyin_Loc = PinyinHeaderIndex[HIndex]*8 + PinyinAddr_Start;
			for(j=0;;j++)
			{
				SpiFlash_Read(buffer,Pinyin_Loc+j*8,8);
				if(buffer[0]!=Pinyin.InputBuf[i])
					return 0;
				if(StrnEqu((u8*)(&Pinyin.InputBuf[i]),buffer,strlen((char*)(&Pinyin.InputBuf[i]))))
				{
					uint32_t pyDataAddr = ((((uint32_t)buffer[6])<<8) | buffer[7]) +PinyinAddr_Start + 3168;
					memset(Pinyin.DataBuf,0,sizeof(Pinyin.DataBuf));
					SpiFlash_Read((u8*)Pinyin.DataBuf,pyDataAddr,sizeof(Pinyin.DataBuf));
					return strlen(Pinyin.DataBuf);
				}
			}
		}
	}
#else
    //HIndex= Input_py(Pinyin.InputBuf);
    //if(HIndex > 0)
    //{
    //	memset(Pinyin.DataBuf,0,sizeof(Pinyin.DataBuf));
    //	locate_allhz(Pinyin.DataBuf);
    //	return strlen(Pinyin.DataBuf);
    //	}
    memset(Pinyin.DataBuf, 0, sizeof(Pinyin.DataBuf));
    HIndex = get_pymb((u8 *)Pinyin.InputBuf, (u8 *)Pinyin.DataBuf);
    HIndex = HIndex;
    return strlen(Pinyin.DataBuf);
#endif
    //	return 0;
}

bool Pinyin_GetPinyinOutData(u8 *OutData)
{
    if (Pinyin.SelData[0] || Pinyin.SelData[1])
    {
        OutData[0] = Pinyin.SelData[0];
        OutData[1] = Pinyin.SelData[1];
        Pinyin.SelData[0] = 0;
        Pinyin.SelData[1] = 0;
        Pinyin.Enable = false;
        return true;
    }
    return true;
}

void Pinyin_Select(void)
{
    memset(Pinyin.SelData, 0, sizeof(Pinyin.SelData));
    memcpy(Pinyin.SelData, Pinyin.DataBuf + Pinyin.SelIndex * 2, 2);
    memset(Pinyin.InputBuf, 0, sizeof(Pinyin.InputBuf));
    memset(Pinyin.DataBuf, 0, sizeof(Pinyin.DataBuf));
    Pinyin.SelIndex = -1;
    Pinyin.SelPage = 0;
}

void UI_HidePinyinBox(Pinyin_p Pinyin)
{
    //Lcd_DrawRectangle(1, 77, 158, 37, 1, COLOR_WIGHT, true, COLOR_WIGHT);
    lcd_api_clear(1, 77, COLOR_WIGHT, 158, 37);
}

void UI_ShowPinyinBox(u8 Flag)
{
    //    static RichText_t Text_Input;
    //    static RichText_t Text_Code;
    if (Flag == 1)
    {
        //        UI_DrawRichText(&Text_Input, 1, 77, 158, 19, COLOR_WIGHT, false);
        //        UI_DrawRichText(&Text_Code, 1, 95, 158, 18, COLOR_WIGHT, false);

        Pinyin_Init();
        Pinyin.Enable = true;
        //Ë¢ĞÂÏÔÊ¾ÊäÈë·¨¿ò
        lcd_api_clear(0, 74, COLOR_WIGHT, 160, 38);
    }
    else if (Pinyin.UpdateFlag)
    {
        u8 i = 0;
        u8 Len = 0;
        u8 Buffer[16];
        u8 ChineseNum = 0;

        Len = strlen(Pinyin.InputBuf);

        Pinyin.UpdateFlag = 0;
        memset(Buffer, 0, sizeof(Buffer));

        if (Pinyin.SelIndex < 0)
        {
            //UI_DisplayText(&Text_Input, 2, 78, 156, 16, FONT_SIZE_MODE1, FONT_ASCII, COLOR_BLUE, COLOR_WIGHT, (u8 *)Pinyin.InputBuf, strlen(Pinyin.InputBuf), true);
            lcd_api_clear_a_line(size_12, 78);
            lcd_api_text_gbk_display(size_12, display_left, 2, 78, (u8 *)Pinyin.InputBuf, COLOR_BLUE, COLOR_WIGHT);
        }
        else
        {
            //UI_DisplayText(&Text_Input, 2, 78, 156, 16, FONT_SIZE_MODE1, FONT_ASCII, COLOR_RED, COLOR_WIGHT, (u8 *)Pinyin.InputBuf, strlen(Pinyin.InputBuf), true);
            lcd_api_text_gbk_display(size_12, display_left, 2, 78, (u8 *)Pinyin.InputBuf, COLOR_RED, COLOR_WIGHT);
        }

        //UI_DrawRichText(&Text_Code, 1, 95, 158, 18, COLOR_WIGHT, false);
        Len = Pinyin_GetStringData();
        if (Len)
        {
            char Temp[6];
            if (Pinyin.SelPage * PY_PAGE_MAX_NUM * 2 >= Len)
                Pinyin.SelPage = Len / (PY_PAGE_MAX_NUM * 2);

            if (Pinyin.SelIndex >= (Len + 1) / 2)
            {
                Pinyin.SelIndex = (Len + 1) / 2 - 1;
            }
#if 0
            if (Pinyin.SelIndex < 0)
            {
                //UI_DisplayText(&Text_Input, 130, 79, 26, 16, FONT_SIZE_MODE0, FONT_GB2312, COLOR_BLUE, COLOR_WIGHT, (unsigned char *)c_delete, 4, true);
                lcd_api_text_gbk_display(size_12, display_left, 160 - 6 * sizeof(c_Delete) - 5, 79, (u8 *)c_Delete, COLOR_BLUE, COLOR_WIGHT);
            }
#endif
            else
            {
                memset(Buffer, 0, sizeof(Buffer));
                DataFormat2((char *)Buffer, "%d/%d", Pinyin.SelPage + 1, Len / (PY_PAGE_MAX_NUM * 2) + 1);
                //UI_DisplayText(&Text_Input, 130, 78, 36, 16, FONT_SIZE_MODE1, FONT_ASCII, COLOR_BLUE, COLOR_WIGHT, (u8 *)Buffer, strlen((char *)Buffer), true);
                lcd_api_text_gbk_display(size_12, display_left, 130, 78, (u8 *)Buffer, COLOR_BLUE, COLOR_WIGHT);
            }

            ChineseNum = Len / 2 - (Pinyin.SelPage * PY_PAGE_MAX_NUM);
            if (ChineseNum > PY_PAGE_MAX_NUM)
                ChineseNum = PY_PAGE_MAX_NUM;

            lcd_api_clear_a_line(size_12, 95);
            for (i = 0; i < ChineseNum; i++)
            {
                Temp[0] = i + 0x31;
                //UI_RcichTextAppendText(&Text_Code, (u8 *)Temp, FONT_ASCII, COLOR_BLACK, 1);
                typewriting_text_gbk_display(size_12, display_left, i * 18, 95, (u8 *)Temp, 1, COLOR_BLUE, COLOR_WIGHT); //ÏÔÊ¾Êı×Ö

                if ((Pinyin.SelIndex >= 0) && (i == (Pinyin.SelIndex % PY_PAGE_MAX_NUM)))
                {
                    //UI_RcichTextAppendText(&Text_Code, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), FONT_GB2312, COLOR_RED, 2);
                    typewriting_text_gbk_display(size_12, display_left, i * 18 + 6, 95, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), 2, COLOR_RED, COLOR_WIGHT);
                }
                else
                {
                    //UI_RcichTextAppendText(&Text_Code, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), FONT_GB2312, COLOR_BLACK, 2);
                    typewriting_text_gbk_display(size_12, display_left, i * 18 + 6, 95, (u8 *)Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2, 2, COLOR_BLUE, COLOR_WIGHT); //ÏÔÊ¾¾ßÌåµÄ×Ö
                }
            }
        }
				else
				{
					//½â¾öÆ´ÒôÊäÈë·¨£¬É¾³ıµ½×îºóÒ»¸ö²»Çå³ı´ıÑ¡×ÖµÄÎÊÌâ
					lcd_api_clear_a_line(size_12, 95);
					ui.page = UI_PAGE_PHONE_NEW_CONTACT;
				}
    }
}

void UI_ShowSymbleBoxItem(Symble_p Symble, u8 Flag)
{
    u8 i = 0;
    u8 symbleData = 0;
    //		RichText_t Text;
    static u8 StartIndex = 0;
    Symble->ShowOn = true;
    //		UI_DrawRichText(&Text,2,91,155,20,COLOR_WIGHT,FALSE);
    //		UI_SetRichTextFont(&Text,COLOR_BLACK,FONT_SIZE_MODE1);

    if (Symble->SelIndex >= Symble_GetSymbleCount())
        Symble->SelIndex = Symble_GetSymbleCount() - 1;

    if (Symble->SelIndex < 0)
        StartIndex = 0;
    else if (StartIndex + 14 < Symble->SelIndex)
        StartIndex = Symble->SelIndex - 14;
    else if (Symble->SelIndex < StartIndex)
        StartIndex = Symble->SelIndex;
    for (i = 0; i < 15; i++)
    {
        symbleData = (u8)Symble_GetSymbleValue((u8)(StartIndex + i));
        if (symbleData)
        {
            if ((StartIndex + i) == Symble->SelIndex)
            {
                lcd_api_clear(2 + i * 10 + 1, 92, COLOR_BLUE, 10, 18);
                //UI_DisplayText_Gb2312_Size1_WightBlue_WithoutFrame(2+i*10+1,92,&symbleData,1);
                lcd_api_text_gbk_display(size_12, display_left, 2 + i * 10 + 1, 92, &symbleData, COLOR_WIGHT, COLOR_BLUE);
            }
            else
            {
                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(2+i*10+1,92,&symbleData,1);
                lcd_api_text_gbk_display(size_12, display_left, 2 + i * 10 + 1, 92, &symbleData, COLOR_BLUE, COLOR_WIGHT);
            }
        }
    }
}

void UI_ShowSymbleBox(Symble_p Symble, u8 Flag)
{
    static int8_t SelIndex = 0;
    if (Flag == 0)
    {
        if (SelIndex != Symble->SelIndex)
        {
            SelIndex = Symble->SelIndex;
            UI_ShowSymbleBoxItem(Symble, 0);
        }
    }
    else if (Flag == 1)
    {
        Symble_Init(Symble, true);
        SelIndex = Symble->SelIndex;
        UI_ShowSymbleBoxItem(Symble, 1);
    }
}

void UI_CloseSymbleBox(Symble_p Symble)
{
    Symble->ShowOn = false;
    Symble->Enable = false;
    lcd_api_clear(2, 91, COLOR_WIGHT, 155, 20);
}

void UI_ShowKeyBoardInputType(KeyBoard_t KeyBoard)
{
#ifdef LANGUAGE_ENGLISH
    const char MsgTable[4][5] = {"123 ", "abc ", "·ûºÅ", "ÖĞÎÄ"};
#else
    const char MsgTable[4][5] = {"123 ", "abc ", "ÖĞÎÄ", "·ûºÅ"};
#endif
    //UI_DisplayText_Gb2312_Size0_WightBlue_WithoutFrame(70, 114, (u8 *)MsgTable[KeyBoard.Type], 4);
    lcd_api_text_gbk_display(size_12, display_left, 70, 114, (u8 *)MsgTable[KeyBoard.Type], COLOR_BACKGROUND, COLOR_FONT);
}

void Symble_Init(Symble_p Symble, bool En)
{
    Symble->ShowOn = false;
    Symble->Enable = En;
    Symble->SelIndex = -1;
    Symble->SelValue = 0;
}

u8 Symble_GetSymbleCount(void)
{
    return sizeof(SymbleTable);
}

char Symble_GetSymbleValue(u8 SymbleIndex)
{
    if (SymbleIndex >= sizeof(SymbleTable))
        return 0;
    return SymbleTable[SymbleIndex];
}
void Symble_SetSelectSymble(Symble_p Symble)
{
    if ((Symble->SelIndex >= 0) && (Symble->SelIndex < sizeof(SymbleTable)))
        Symble->SelValue = SymbleTable[Symble->SelIndex];
    else
        Symble->SelValue = 0;
}

char Symble_GetSelectedSymble(Symble_p Symble)
{
    char ret = Symble->SelValue;
    Symble->SelValue = 0;
    return ret;
}

#if 1

void pinyin_process(void)
{
    static u8 keyboard_number_old = 0;

    if (ui.page == UI_PAGE_PHONE_NEW_CONTACT || ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON)
    {
        if (KeyBoard.state == KB_STATE_OFF)
        {
            Pinyin.Enable = false;
            Symble.Enable = false;
            Symble_Init(&Symble, false);
        }
        if (Pinyin.Enable == true)
            UI_ShowPinyinBox(0);

        if (Symble.Enable == true)
            UI_ShowSymbleBox(&Symble, 0);

        //¼üÖµË¢ĞÂÊ±½øÒ»´Î
        if (keyboard_number_old != get_keyboard_number())
        {
            keyboard_number_old = get_keyboard_number();

            if (keyboard_number_old != 0x00)
            {
                //Èç¹û¿ªÆôÆ´ÒôÊäÈë·¨£¬¼ì²âÆ´ÒôÊäÈë»òÕßÑ¡
                if (KeyBoard.state == KB_STATE_ON)
                {
                    if (KeyBoard.Type == KB_PINYIN)
                    {
                        //if((Key.Code>=K1)&&(Key.Code<=K9))
                        if (get_keyboard_number() >= 0x31 && get_keyboard_number() <= 0x39)
                        {
                            ui.page = UI_PAGE_PHONE_NEW_CONTACT_SON; //ÊÇÆ´Òô½øÈëÊäÈë·¨´°¿Ú
                            if (Pinyin.Enable == false)
                            {
                                UI_ShowPinyinBox(1);
                            }
                            if (Pinyin.Enable == true)
                            {
                                if (Pinyin.SelIndex < 0)
                                {
                                    KeyBoard_GetPinyinInputChar();
                                }
                                else
                                {
                                    //uint8_t index = Key.Code-K1;
                                    u8 index = get_keyboard_number() - 0x31;

                                    if (index < PY_PAGE_MAX_NUM)
                                    {
                                        Pinyin.SelIndex = Pinyin.SelPage * PY_PAGE_MAX_NUM + index;
                                        Pinyin_Select();
                                        if (Pinyin.SelIndex != 0xff)
                                        {
                                            //Ë¢ĞÂÏÔÊ¾ÊäÈë·¨¿ò
                                            lcd_api_clear(0, 74, COLOR_WIGHT, 160, 38);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            if (get_keyboard_number() == '#')
                            {
                                ui.page = UI_PAGE_PHONE_NEW_CONTACT; //²»ÊÇÆ´ÒôÍË³öÊäÈë·¨´°¿Ú

                                //Çå³ıÏà¹Ø±êÖ¾Î»
                                Pinyin_Init();
                            }
                            KeyBoard_InputChar();
                        }
                    }
                    //Êı×Ö¼üÅÌ
                    else //if(KeyBoard->Type==KB_NUMBER)
                    {
                        KeyBoard_InputChar();
                        keyboard_number_old = 0;
                    }
                }
            }
        }
    }
}

void ui_phone_new_contact(ui_change_type opt)
{
    static signed char sel = 0;
    static signed char lastSel = -1;
    char InputChar[3];
    char ReSelFlag = 0;

    KeyBoard.state = KB_STATE_ON;
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //µÚÒ»´Î½øÈë
        ui.page = UI_PAGE_PHONE_NEW_CONTACT;
        ui_clear();
        ui_show_title((u8 *)c_New_contact);
        ui_show_bottom_button(BUTTOM_OK_BACK);

        sel = 0;
        lastSel = -1;

        memset(&new_caontact, 0x00, sizeof(one_contact_item_t));
        Pinyin_Init();
        KeyBoard.Type = KB_ENGLISH;
        Symble_Init(&Symble, false);
        UI_ShowKeyBoardInputType(KeyBoard);
        break;
    case UI_OPT_KEY_UP: //ÉÏÇĞ»»ÏîÄ¿
        sel = (sel + 1) % 2;
        break;
    case UI_OPT_KEY_DOWN: //ÏÂÇĞ»»ÏîÄ¿
        sel = (sel + 1) % 2;
        break;
    case UI_OPT_KEY_ENTER: //È·ÈÏ±£´æÊı¾İ
                           //Æ´ÒôÊäÈëÄ£Ê½
        if (KeyBoard.Type != KB_PINYIN)
        {
            //Çå³ıÏà¹Ø±êÖ¾Î»
            Pinyin_Init();
        }

        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex >= 0)
            {
                Pinyin_Select();
                //Ñ¡ÖĞºóÇå³ıÊäÈë·¨½çÃæ
            }
        }
        //ÌØÊâ·ûºÅÑ¡Ôñ
        else if (Symble.Enable == true)
        {
            Symble_SetSelectSymble(&Symble);
        }
        else
        {
            KeyBoard.state = KB_STATE_OFF;
            if ((strlen((char *)new_caontact.name) > 0) && (strlen((char *)new_caontact.phone) > 0))
            {
                //´æĞÂÁªÏµÈËµ½flash
                deposit_new_contact();
            }
            ui_phone(UI_OPT_PART_REFRESH);
        }
        break;
    case UI_OPT_KEY_BACK: //·µ»ØÉÏÒ»¼¶
        ui_phone(UI_OPT_PART_REFRESH);
        break;
    case UI_OPT_KEY_KEYBOARD:

        if (ui.page == UI_PAGE_PHONE_NEW_CONTACT || ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON)
        {
            //ÊµÊ±Ë¢ĞÂ
            switch (sel)
            {
            case 0: //¹â±êÔÚĞÕÃû
                memset(InputChar, 0x00, sizeof(InputChar));
                ReSelFlag = 0;
                if (KeyBoard.Type == KB_SYMBLE)
                {
                    if (Symble.ShowOn == false)
                        UI_ShowSymbleBox(&Symble, 1);
                    InputChar[0] = Symble_GetSelectedSymble(&Symble);
                    if (InputChar[0])
                        UI_CloseSymbleBox(&Symble);
                }
                else if (KeyBoard.Type == KB_PINYIN)
                {
                    Pinyin_GetPinyinOutData((u8 *)InputChar);
                }
                else
                {
                    /* code */
                }

                if (InputChar[0] == 0)
                {
                    if (KeyBoard_GetInputChar(&InputChar[0], &ReSelFlag))
                    {
                        if (InputChar[0] == '#') //ÇĞ»»ÊäÈë·¨
                        {
#ifdef LANGUAGE_ENGLISH
                            if (KeyBoard.Type == KB_PINYIN)
                            {
                                lastSel = -1;
                                Symble_Init(&Symble, false);
                            }
                            else if (KeyBoard.Type == KB_SYMBLE)
                            {
                                lastSel = -1;
                            }
#else

                            if (KeyBoard.Type == KB_SYMBLE)
                            {
                                lastSel = -1;
                                Symble_Init(&Symble, false);
                            }
                            else if (KeyBoard.Type == KB_PINYIN)
                            {
                                lastSel = -1;
                            }
#endif
                            if (KeyBoard.Type == KB_SYMBLE)
                            {
                                KeyBoard.Type = KB_NUMBER;
                            }
                            else
                            {
                                KeyBoard.Type++;
                                if (KeyBoard.Type == KB_SYMBLE)
                                {
                                    KeyBoard.Type = KB_NUMBER;
                                }
                            }
                            UI_ShowKeyBoardInputType(KeyBoard);
                            InputChar[0] = 0;
                        }
                        else if (InputChar[0] == '*') //É¾³ı
                        {
                            u8 lenPinyinInBuf;
                            lenPinyinInBuf = strlen(Pinyin.InputBuf);
                            if ((KeyBoard.Type == KB_PINYIN) && (lenPinyinInBuf > 0))
                            {
                                if (Pinyin.SelIndex == -1)//½â¾öÆ´ÒôÊäÈë·¨½çÃæ°´ÏÂ¼ü½øÈëÑ¡Ôñ´°¿ÚÉ¾³ıÏÔÊ¾Òì³£µÄÎÊÌâ£¨´¦ÓÚ´ıÑ¡×Ö½çÃæ°´É¾³ıÊ§Ğ§£©
                                {
                                    Pinyin.InputBuf[lenPinyinInBuf - 1] = 0x00;
                                    Pinyin.UpdateFlag = 1;
                                }
                            }
                            else if (strlen((char *)new_caontact.name) > 0)
                            {
                                if (new_caontact.name[strlen((char *)new_caontact.name) - 1] >= 0xA0)
                                {
                                    new_caontact.name[strlen((char *)new_caontact.name) - 1] = 0;
                                }
                                new_caontact.name[strlen((char *)new_caontact.name) - 1] = 0;
                                lcd_api_clear(17, 55, COLOR_WIGHT, 126, 18);
                                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 56, (u8 *)new_caontact.name, strlen((const char *)new_caontact.name));
                                lcd_api_text_gbk_display(size_12, display_left, 18, 56, (u8 *)new_caontact.name, COLOR_BLUE, COLOR_WIGHT);
                            }
                            InputChar[0] = 0;
                        }
                        else
                        {
                        }
                    }
                }
                if (InputChar[0] != 0)
                {
                    if (strlen((char *)new_caontact.name) > 0)
                    {
                        if (ReSelFlag == true)
                        {
                            new_caontact.name[strlen((char *)new_caontact.name) - 1] = 0;
                        }
                    }
                    if (strlen((char *)new_caontact.name) < (sizeof(new_caontact.name) - strlen(InputChar)))
                    {
                        new_caontact.name[strlen((char *)new_caontact.name)] = InputChar[0];
                        if (InputChar[1])
                        {
                            new_caontact.name[strlen((char *)new_caontact.name)] = InputChar[1];
                        }
                        //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 56, (u8 *)new_caontact.name, strlen((const char *)new_caontact.name));
                        lcd_api_text_gbk_display(size_12, display_left, 18, 56, (u8 *)new_caontact.name, COLOR_BLUE, COLOR_WIGHT);
                    }
                }

                //ÏÔÊ¾¹â±ê
                //        RichText.Cousor.LocX = 18 + strlen((char*)new_caontact.name) * 8;
                //        RichText.Cousor.LocY = 56;
                //        UI_ShowTextCousor(&RichText, 0);
                break;
            case 1: //¹â±êÔÚºÅÂë
                if (KeyBoard_GetInputChar(&InputChar[0], &ReSelFlag))
                {
                    if (InputChar[0] == '*')
                    {
                        if (strlen((char *)new_caontact.phone) > 0)
                        {
                            new_caontact.phone[strlen((char *)new_caontact.phone) - 1] = 0;
                            lcd_api_clear(17, 93, COLOR_WIGHT, 126, 18);
                            //UI_DisplayText_Ascii_Size1_BlueWight_WithoutFrame(18, 94, (u8 *)new_caontact.phone, strlen((const char *)new_caontact.phone));
                            lcd_api_text_gbk_display(size_12, display_left, 18, 94, (u8 *)new_caontact.phone, COLOR_BLUE, COLOR_WIGHT);
                        }
                    }
                    else if (InputChar[0] == '#')
                    {
                    }
                    else if (strlen((char *)new_caontact.phone) < (sizeof(new_caontact.phone) - 1))
                    {
                        if (InputChar[0] != 0)
                        {
                            new_caontact.phone[strlen((char *)new_caontact.phone)] = InputChar[0];
                            //UI_DisplayText_Ascii_Size1_BlueWight_WithoutFrame(18, 94, (u8 *)new_caontact.phone, strlen((const char *)new_caontact.phone));
                            lcd_api_text_gbk_display(size_12, display_left, 18, 94, (u8 *)new_caontact.phone, COLOR_BLUE, COLOR_WIGHT);
                        }
                    }
                }

                //ÏÔÊ¾¹â±ê
                //        RichText.Cousor.LocX = 18 + strlen((char*)new_caontact.phone) * 8;
                //        RichText.Cousor.LocY = 94;
                //        UI_ShowTextCousor(&RichText, 0);
                break;
            }
        }

        break;

    default:
        break;
    }

    if (ui.page == UI_PAGE_PHONE_NEW_CONTACT || ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON)
    {
        //Ö»½øÒ»´Î
        if (sel != lastSel)
        {
            lastSel = sel;
            switch (sel)
            {
            case 0:
                //ÏÔÊ¾¹â±ê
                //            RichText.Cousor.LocX = 18 + strlen((char*)new_caontact.name) * 8;
                //            RichText.Cousor.LocY = 56;
                //            UI_ShowTextCousor(&RichText, 1);

                lcd_api_clear(0, 36, COLOR_BLUE, 160, 38);
                //UI_DisplayText_Gb2312_Size1_WightBlue_WithoutFrame(5, 37, (unsigned char *)c_name, strlen(c_name));
                lcd_api_text_gbk_display(size_12, display_left, 5, 37, (u8 *)c_name, COLOR_WIGHT, COLOR_BLUE);

                lcd_api_clear(17, 55, COLOR_WIGHT, 126, 18);
                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 56, (u8 *)new_caontact.name, strlen((const char *)new_caontact.name));
                lcd_api_text_gbk_display(size_12, display_left, 18, 56, (u8 *)new_caontact.name, COLOR_BLUE, COLOR_WIGHT);

                lcd_api_clear(0, 74, COLOR_WIGHT, 160, 38);
                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(5, 75, (unsigned char *)c_number, strlen(c_number));
                lcd_api_text_gbk_display(size_12, display_left, 5, 75, (u8 *)c_number, COLOR_BLUE, COLOR_WIGHT);

                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 94, (u8 *)new_caontact.phone, strlen((const char *)new_caontact.phone));
                lcd_api_text_gbk_display(size_12, display_left, 18, 94, (u8 *)new_caontact.phone, COLOR_BLUE, COLOR_WIGHT);

                break;
            case 1:
                //ÏÔÊ¾¹â±ê
                //            RichText.Cousor.LocX = 18 + strlen((char*)new_caontact.phone) * 8;
                //            RichText.Cousor.LocY = 94;
                //            UI_ShowTextCousor(&RichText, 1);

                KeyBoard.Type = KB_NUMBER;
                Symble_Init(&Symble, false);
                UI_ShowKeyBoardInputType(KeyBoard);
                lcd_api_clear(0, 36, COLOR_WIGHT, 160, 38);
                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(5, 37, (unsigned char *)c_name, strlen(c_name));
                lcd_api_text_gbk_display(size_12, display_left, 5, 37, (u8 *)c_name, COLOR_BLUE, COLOR_WIGHT);

                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 56, (u8 *)new_caontact.name, strlen((const char *)new_caontact.name));
                lcd_api_text_gbk_display(size_12, display_left, 18, 56, (u8 *)new_caontact.name, COLOR_BLUE, COLOR_WIGHT);

                lcd_api_clear(0, 74, COLOR_BLUE, 160, 38);
                //UI_DisplayText_Gb2312_Size1_WightBlue_WithoutFrame(5, 75, (unsigned char *)c_number, strlen(c_number));
                lcd_api_text_gbk_display(size_12, display_left, 5, 75, (u8 *)c_number, COLOR_WIGHT, COLOR_BLUE);

                lcd_api_clear(17, 93, COLOR_WIGHT, 126, 18);
                //UI_DisplayText_Gb2312_Size1_BlueWight_WithoutFrame(18, 94, (u8 *)new_caontact.phone, strlen((const char *)new_caontact.phone));
                lcd_api_text_gbk_display(size_12, display_left, 18, 94, (u8 *)new_caontact.phone, COLOR_BLUE, COLOR_WIGHT);

                break;
            }
        }
    }
}
#endif

void ui_phone_new_contact_son_page(ui_change_type opt)
{
    switch (opt)
    {
    case UI_OPT_FIRST_REFRESH: //µÚÒ»´Î½øÈë
        ui.page = UI_PAGE_PHONE_NEW_CONTACT_SON;
        break;
    case UI_OPT_KEY_UP: //ÉÏÇĞ»»ÏîÄ¿
        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex > 0)
            {
                Pinyin.SelIndex--;
                Pinyin.SelPage = (Pinyin.SelIndex) / PY_PAGE_MAX_NUM;
            }
            Pinyin.UpdateFlag = 1;
        }
        break;
    case UI_OPT_KEY_DOWN: //ÏÂÇĞ»»ÏîÄ¿
        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex < 0)
            {
                Pinyin.SelIndex = 0;
                Pinyin.SelPage = 0;
            }
            else
            {
                Pinyin.SelIndex++;
                Pinyin.SelPage = (Pinyin.SelIndex) / PY_PAGE_MAX_NUM;
            }
            Pinyin.UpdateFlag = 1;
        }
        break;
    case UI_OPT_KEY_ENTER: //È·ÈÏ±£´æÊı¾İ
        //Æ´ÒôÊäÈëÄ£Ê½
        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex >= 0)
            {
                Pinyin_Select();
            }
        }
        //ÌØÊâ·ûºÅÑ¡Ôñ
        else if (Symble.Enable == true)
        {
            Symble_SetSelectSymble(&Symble);
        }
        else
        {
        }

        if ((Pinyin.Enable == false) && (Symble.Enable == false))
        {
            KeyBoard.state = KB_STATE_OFF;
        }

        if (Pinyin.SelIndex != 0xff)
        {
            //Ë¢ĞÂÏÔÊ¾ÊäÈë·¨¿ò
            lcd_api_clear(0, 74, COLOR_WIGHT, 160, 38);
        }
        ui.page = UI_PAGE_PHONE_NEW_CONTACT; //°´ÁËÈ·ÈÏ¼üºó£¬¹â±ê·µ»ØÉÏ¼¶Ò³Ãæ

        break;
    case UI_OPT_KEY_BACK: //·µ»ØÉÏÒ»¼¶
        ui_menu(UI_OPT_PART_REFRESH);
        break;
    default:
        break;
    }
}

void MemClear(u8 *Mem, u16 Size)
{
    u16 i = 0;
    for (i = 0; i < Size; i++)
        Mem[i] = 0;
}
const char HexTable[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char *itoa(long num, char *str, uint8_t radix)
{
    int i = 0;
    int j = 0;
    int c = 0;
    char *ptr = str;

    do
    {
        c++;
        if (num < radix)
        {
            *ptr++ = HexTable[num % radix];
            if ((c % 2) && radix == 16)
            {
                *ptr++ = '0';
            }
            *ptr = '\0';
            break;
        }
        *ptr++ = HexTable[num % radix];
        num /= radix;
    } while (num);

    j = ptr - str - 1;
    for (i = 0; i < (ptr - str) / 2; i++)
    {
        int temp = str[i];
        str[i] = str[j];
        str[j--] = temp;
    }
    return str;
}
static void DataFormat2(char *Buffer, char *fmt, long data1, long data2)
{
    u8 i = 0;
    u8 paraindex = 0;
    u8 Len_fmt = 0;
    long data = 0;
    char buffer[20];
    u8 Index = 0;
    Len_fmt = strlen(fmt);
    do
    {
        if ((((char)fmt[i]) == '%') && (((((char)fmt[i + 1]) == 'd') || ((char)fmt[i + 1]) == 'x')))
        {
            u8 j = 0;
            MemClear((u8 *)buffer, 20);
            if (paraindex == 0)
            {
                data = data1;
            }
            else if (paraindex == 1)
                data = data2;
            else if (paraindex == 2)
                data = NULL;
            else
                break;
            if (((char)fmt[i + 1]) == 'd')
            {
                itoa(data, buffer, 10);
            }
            else
            {
                itoa(data, buffer, 16);
            }
            paraindex++;
            for (j = 0; j < strlen(buffer); j++)
            {
                Buffer[Index++] = buffer[j];
                if (Index == 0)
                    break;
            }
            i += 2;
        }
        else
        {
            Buffer[Index++] = fmt[i++];
        }
        if (Index == 0)
            break;

    } while (i < Len_fmt);
}

//[0]:¸üĞÂ±êÖ¾  [1]: ÖØÑ¡±êÖ¾ [2]£ºÊäÈë×Ö·û
bool KeyBoard_GetInputChar(char *CharData, char *ReSelFlag)
{
    //ÓĞÊı¾İ¸üĞÂ
    if (KeyBoard.InCharBuf[0])
    {
        KeyBoard.InCharBuf[0] = 0; //Çå³ıÊı¾İ¸üĞÂ±êÖ¾
        *ReSelFlag = KeyBoard.InCharBuf[1];
        *CharData = KeyBoard.InCharBuf[2];
        return true;
    }
    return false;
}

char KeyBoard_GetPinyinInputChar(void)
{
    static u32 DifCharCnt = 0;
    static u8 KeyCharIndex = 0;
    static u8 PreKeyCode = 0;
    u8 KeyCode = 0;

    KeyCode = get_keyboard_number();

    u8 RepeatFlag = 0;
    if (PreKeyCode == KeyCode)
    {
        if (DifCharCnt + 2000 > SysTick_Cnt)
        {
            KeyCharIndex = (KeyCharIndex + 1) % 6;
            RepeatFlag = 1;
        }
        else
            KeyCharIndex = 0;
    }
    else
    {
        KeyCharIndex = 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    PreKeyCode = KeyCode;
    DifCharCnt = SysTick_Cnt;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 1~ 9 ¼ü ±ê×¢Æ´Òô×ÖÄ¸
    if ((KeyCode >= 0x31) && (KeyCode <= 0x39))
    {
        char pinyin_char = 0;
        u8 len = strlen(Pinyin.InputBuf);
        if (RepeatFlag)
        {
            if (len)
                len--;
            pinyin_char = KeyBoard_KeyChar(KeyCharIndex); //3

            if ((pinyin_char >= 'A') && (pinyin_char <= 'Z'))
                pinyin_char += 0x20;

            Pinyin.InputBuf[len] = pinyin_char;
            Pinyin.UpdateFlag = 1;
        }
        else
        {
            if (len < 6)
            {
                pinyin_char = KeyBoard_KeyChar(KeyCharIndex); //4
                if ((pinyin_char >= 'A') && (pinyin_char <= 'Z'))
                    pinyin_char += 0x20;
                Pinyin.InputBuf[len] = pinyin_char;
                Pinyin.UpdateFlag = 1;
            }
        }
    }
    return 0;
}

void KeyBoard_InputChar(void)
{
    static u32 DifCharCnt = 0;
    static u8 KeyCharIndex = 0;
    static u8 PreKeyCode = 0;
    u8 KeyCode = 0;

    KeyCode = get_keyboard_number();

    static u8 Temp = 0;
    u8 RepeatFlag = 0;
    u16 dlyRepeat;
    if (KeyCode != 0) //µ±¼üÖµ=¿ÕÊ±£¬Ìø¹ı¼ì²â£¬½â¾ö=0Ê±°´1ÇĞ»»²»ÁËabcµÄÎÊÌâ
    {
        if (PreKeyCode == KeyCode)
        {
            while (1)
            {
                if ((KeyCode >= 0x31) && (KeyCode <= 0x39) && (KeyBoard.Type == KB_PINYIN || KeyBoard.Type == KB_ENGLISH))
                {
                    dlyRepeat = 3000 / 2;
                }
                else
                {
                    dlyRepeat = 200 / 2;
                }

                if ((DifCharCnt + dlyRepeat) > SysTick_Cnt)
                {
                    KeyCharIndex = (KeyCharIndex + 1) % 6;
                    RepeatFlag = 1;
                    //                if (KeyBoard_KeyChar(KeyCharIndex)) //1
                    break;
                }
                else
                {
                    KeyCharIndex = 0;
                    break;
                }
            }
        }
        else
        {
            KeyCharIndex = 0;
        }

        PreKeyCode = KeyCode;
        DifCharCnt = SysTick_Cnt;

        Temp = KeyBoard_KeyChar(KeyCharIndex); //2
        if (Temp)
        {
            KeyBoard.InCharBuf[0] = 1; //¸üĞÂ

            KeyBoard.InCharBuf[1] = RepeatFlag; //ÊÇ·ñÖØ¸´

            KeyBoard.InCharBuf[2] = Temp; //Êµ¼ÊÊäÈë×Ö·û
        }
        else
        {
            KeyBoard.InCharBuf[0] = 0; //Î´¸üĞÂ
        }
    }
}

char KeyBoard_KeyChar(u8 CharIndex)
{
    u8 KeyCode = 0;

    KeyCode = get_keyboard_number();

    //Êı×Ö¼üÅÌ
    if (KeyBoard.Type == KB_NUMBER)
    {
        //Êı×Ö¼ü 1~9
        if (KeyCode >= 0x31 && KeyCode <= 0x39)
        {
            keyboard_number_clear();
            return KeyCode;
        }
        else
        {
            keyboard_number_clear();
            return KeyCode; //'0''*''#'
        }
    }
    else if ((KeyBoard.Type == KB_PINYIN) || (KeyBoard.Type == KB_ENGLISH))
    {

        if (KeyCode >= 0x31 && KeyCode <= 0x39) //Æ´Òô¼üÅÌ & Ó¢ÎÄ¼üÅÌ
        {
            keyboard_number_clear();
            return KeyPinyinArray[KeyCode - 0x30 - 1][CharIndex];
        }
        else
        {
            keyboard_number_clear();
            return KeyCode; //'0''*''#'
        }
    }
    else
    {
        keyboard_number_clear();
        return KeyCode; //'0''*''#'
    }
}

void typewriting_text_gbk_display(SIZE_TYPE size, DISPLAY_TYPE display_type, u16 LocX, u16 LocY, u8 *Gb2312Data, u8 data_len, u16 uiFontColor, u16 uiBackColor)
{
    u8 single_data_size = 0;
    u16 x = 0;

    if (size == size_12)
    {
        single_data_size = 6;
        if (data_len > 26)
        {
            data_len = 26; //size_12×î¶àÏÔÊ¾26¸öÓ¢ÎÄ»ò13¸öÖĞÎÄ
        }
    }
    else if (size == size_16)
    {
        single_data_size = 8;
        if (data_len > 19)
        {
            data_len = 19; //size_16×î¶àÏÔÊ¾19¸öÓ¢ÎÄ»ò9¸öÖĞÎÄ
        }
    }
    else
    {
    }

    if (display_type == display_middle) //¾ÓÖĞ
    {
        x = (160 - LocX - single_data_size * data_len) / 2;
    }
    else //¾ÓÓÒºÍ×Ô¶¨Òå
    {
        x = LocX;
    }

    if (size == size_12)
    {
        typewriting_display_chinese_11x12(x, LocY, (char *)Gb2312Data, data_len, uiBackColor, uiFontColor);
    }
    else if (size == size_16)
    {
        typewriting_display_chinese_11x12(x, LocY, (char *)Gb2312Data, data_len, uiBackColor, uiFontColor);
    }
    else
    {
        /* code */
    }
}

void typewriting_display_chinese_11x12(u16 x, u16 y, char *gb2312_dat, u16 lenth, u16 black_color, u16 text_color)
{
    u16 i;
    u8 disp_buf[32] = {0};

    for (i = 0; i < lenth; i++)
    {
        if (gb2312_dat[i] >= 0x20 && gb2312_dat[i] <= 0x7e)
        {
            font_get_ascii_pixel_data_6x12(disp_buf, *(gb2312_dat + i));
            lcd_app_display_font_data(x + i * 6, y, disp_buf, 6, 8, text_color, black_color);
            lcd_app_display_font_data(x + i * 6, y + 8, &disp_buf[6], 6, 5, text_color, black_color);
        }
        else
        {
            if (font_get_gb2312_pixel_data_11x12(disp_buf, (u8 *)(gb2312_dat + i)))
            {
                lcd_app_display_font_data(x + i * 6, y, disp_buf, 12, 8, text_color, black_color);
                lcd_app_display_font_data(x + i * 6, y + 8, &disp_buf[12], 12, 4, text_color, black_color);
                i++;
            }
            else //³ö´íÁË£¬ÕÒ²»µ½×Ö¿â
            {
#ifdef JIANPUZHAI
                uin_dat16 = (gb2312_dat[i]) + (gb2312_dat[(i + 1)] << 8); //0x17¡¢0x80ºÏ³ÉÎª0x1780
                if (uin_dat16 >= 0x1780 && uin_dat16 <= 0x17FF)
                {
                    lcd_disp_bit1bmp_SingleByte(x + i * 8, y, gmy_16_16_bmp_data[uin_dat16 - 0x1780], 16, 16, text_color, black_color);
                }
                i++;
#endif
            }
        }
    }
}
