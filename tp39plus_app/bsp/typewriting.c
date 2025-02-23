//���뷨����������������ֱ�Ӵ���39P��ֲ�������˽�����

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
/*ƴ�����뷨��ѯ���*/
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

//"ƴ�����뷨�������б�"
const unsigned char PY_mb_space[] = {""}; //�� �� ��ST_M8._C N
const unsigned char PY_mb_a[] = {"����߹����ﹺǰ���������"};
const unsigned char PY_mb_ai[] = {"���������������������������������������������"};
const unsigned char PY_mb_an[] = {"������������������������������������㳧"};
const unsigned char PY_mb_ang[] = {"������"};
const unsigned char PY_mb_ao[] = {"���°����������ð�����������������������������"};
const unsigned char PY_mb_b[] = {"���������رϱ�Ȳ���ٱ������"};
const unsigned char PY_mb_ba[] = {"�Ѱ˰ɰͰΰ԰հְӰŰưǰȰʰ̰ϰа���������������"};
const unsigned char PY_mb_bai[] = {"�ٰװܰڲ��ذ۰ݰ�����"};
const unsigned char PY_mb_ban[] = {"���������߰����������������"};
const unsigned char PY_mb_bang[] = {"���������������������"};
const unsigned char PY_mb_bao[] = {"�����������������������ڱ��ٰ�����������������������������"};
const unsigned char PY_mb_bei[] = {"�������������������۱������������������������������������"};
const unsigned char PY_mb_ben[] = {"�����������������"};
const unsigned char PY_mb_beng[] = {"�ñ������±ı����"};
const unsigned char PY_mb_bi[] = {"�ȱرܱձٱʱڱ۱ϱ˱ƱұǱ��رɱ̱ͱбѱӱԱֱױݷ���ذ��������ݩ��޵��������������������������������������������"};
const unsigned char PY_mb_bian[] = {"��߱������ޱ��������������������������������˰�"}; //�� �� ��ST_M8._C N
const unsigned char PY_mb_biao[] = {"�������������������������"};
const unsigned char PY_mb_bie[] = {"�������"};
const unsigned char PY_mb_bin[] = {"������������������������������"};
const unsigned char PY_mb_bing[] = {"����������������������������"};
const unsigned char PY_mb_bo[] = {"�����������������������ز������������������������������������������������"};
const unsigned char PY_mb_bu[] = {"�������������Ҳ�����������߲�����������"};
const unsigned char PY_mb_c[] = {"���ɳ̳²̲ܴ޳��ҳس���᯴Ӵ˴����������Ŵβ�"};
const unsigned char PY_mb_ca[] = {"��������"};
const unsigned char PY_mb_cai[] = {"�ɲŲĲ˲Ʋòʲ²ǲȲ�"};
const unsigned char PY_mb_can[] = {"�βвϲӲͲѲ����������"};
const unsigned char PY_mb_cang[] = {"�زֲԲղ�"};
const unsigned char PY_mb_cao[] = {"�ݲ۲ٲڲ��������"};
const unsigned char PY_mb_ce[] = {"��߲�����"};
const unsigned char PY_mb_cen[] = {"����"};
const unsigned char PY_mb_ceng[] = {"��������"};
const unsigned char PY_mb_ch[] = {"��������³̳ǳ��س���"};
const unsigned char PY_mb_cha[] = {"�������������ɲ���������������������"};
const unsigned char PY_mb_chai[] = {"�����٭�����"};
const unsigned char PY_mb_chan[] = {"��������������������������������������������������"};
const unsigned char PY_mb_chang[] = {"�����������������������������������������������������"};
const unsigned char PY_mb_chao[] = {"�������������������½���������"};
const unsigned char PY_mb_che[] = {"�����߳�����������"};
const unsigned char PY_mb_chen[] = {"�³��Ƴĳ������򳻳����������������������"};
const unsigned char PY_mb_cheng[] = {"�ɳ̳Ƴǳг˳�ʢ�ųϳȳͳγѳҳ�ة���������������������"};
const unsigned char PY_mb_chi[] = {"�ֳ߳ݳԳ�سٳ��ܳճ׳ڳ۳޳�����ܯ��߳������������������������������"};
const unsigned char PY_mb_chong[] = {"�ֳ��س���ӿ������������"};
const unsigned char PY_mb_chou[] = {"���������������ٱ������"};
const unsigned char PY_mb_chu[] = {"���������������������������������ءۻ���������������������"};
const unsigned char PY_mb_chuai[] = {"�����������"};
const unsigned char PY_mb_chuan[] = {"������������������������"};
const unsigned char PY_mb_chuang[] = {"��������������"};
const unsigned char PY_mb_chui[] = {"������׵��������"};
const unsigned char PY_mb_chun[] = {"��������������ݻ����"};
const unsigned char PY_mb_chuo[] = {"����������"};
const unsigned char PY_mb_ci[] = {"�˴δ̴ŴƴʴĴôǴȴɴ���������������"};
const unsigned char PY_mb_cong[] = {"�ӴԴϴдѴ����������"};
const unsigned char PY_mb_cou[] = {"������"};
const unsigned char PY_mb_cu[] = {"�ִٴ״����������������"};
const unsigned char PY_mb_cuan[] = {"�۴ڴ�����ߥ����"};
const unsigned char PY_mb_cui[] = {"�ߴ����˥�޴�������������"}; //�� �� ��ST_M8._C N
const unsigned char PY_mb_cun[] = {"���������"};
const unsigned char PY_mb_cuo[] = {"�������������������������"};
const unsigned char PY_mb_d[] = {"�Ĵ�ضŶ��δ��˶��ҵԵ�絳���ȵ��ӵ����"};
const unsigned char PY_mb_da[] = {"������������������������������"};
const unsigned char PY_mb_dai[] = {"�������������������������ܤ߰߾����������"};
const unsigned char PY_mb_dan[] = {"��������ʯ������������������������������������������"};
const unsigned char PY_mb_dang[] = {"���������������������"};
const unsigned char PY_mb_dao[] = {"������������������������߶���������"};
const unsigned char PY_mb_de[] = {"�ĵõصµ��"};
const unsigned char PY_mb_deng[] = {"�ȵƵǵ˳εŵɵ������������"};
const unsigned char PY_mb_di[] = {"�ĵصڵ͵е���۵ֵεܵݵ̵ϵѵҵӵԵյٵ�ص��ڮ��ۡ��ݶ���������������������"};
const unsigned char PY_mb_dia[] = {"��"};
const unsigned char PY_mb_dian[] = {"������ߵ���������������������������"};
const unsigned char PY_mb_diao[] = {"���������������������"};
const unsigned char PY_mb_die[] = {"����������������ܦ�������������"};
const unsigned char PY_mb_ding[] = {"������������������������������������"};
const unsigned char PY_mb_diu[] = {"����"};
const unsigned char PY_mb_dong[] = {"����������������������������������"};
const unsigned char PY_mb_dou[] = {"������������������������"};
const unsigned char PY_mb_du[] = {"�ȶ��������ɶŶ¶ƶٶ����öĶǶ�ܶ�������������"};
const unsigned char PY_mb_duan[] = {"�϶˶ζ̶Ͷ������"};
const unsigned char PY_mb_dui[] = {"�ԶӶѶض�������"};
const unsigned char PY_mb_dun[] = {"�ֶܶٶ׶ضնڶ۶���������������"};
const unsigned char PY_mb_duo[] = {"��ȶ��޶߶������������������������"};
const unsigned char PY_mb_e[] = {"�����������������������ج������ݭ��������������������"};
const unsigned char PY_mb_ei[] = {"��"};
const unsigned char PY_mb_en[] = {"������"};
const unsigned char PY_mb_er[] = {"����������������٦���������"};
const unsigned char PY_mb_f[] = {"�ַ��������뷴�Ÿ�����Ƿ⸻��"};
const unsigned char PY_mb_fa[] = {"��������������������"};
const unsigned char PY_mb_fan[] = {"����������������������������������ެ����������"};
const unsigned char PY_mb_fang[] = {"���ŷ��÷��ķ·�������������������"};
const unsigned char PY_mb_fei[] = {"�ǷʷɷѷϷηзƷ˷ȷ̷�������������������������������"};
const unsigned char PY_mb_fen[] = {"�ַ۷ܷݷ�׷ҷ߷ӷԷշطٷڷ������������"};
const unsigned char PY_mb_feng[] = {"�����������������ٺۺ��������"};
const unsigned char PY_mb_fo[] = {"��"};
const unsigned char PY_mb_fou[] = {"���"};
const unsigned char PY_mb_fu[] = {"���������򸺸����������������������������������𸿸��������������������������������������������ۮܽ������ݳ����߻����������������������������������������������"};
const unsigned char PY_mb_g[] = {"���������߹���𹢹����ʸɹżֹȹ˹عܸ��﹫�����Ĺ����񹲸�"};
const unsigned char PY_mb_ga[] = {"�и��¸����������"}; //�� �� ��ST_M8._C N
const unsigned char PY_mb_gai[] = {"�ĸøǸŸƸȽ�ؤ�������"};
const unsigned char PY_mb_gan[] = {"�ɸ˸иҸϸʸθѸ̸͸�Ǭ������ߦ����������������"};
const unsigned char PY_mb_gang[] = {"�ոָ׸ٸڸ۸ܸԸؿ������"};
const unsigned char PY_mb_gao[] = {"�߸����ݸ޸���غھ۬޻��������"};
const unsigned char PY_mb_ge[] = {"������ϸ�����Ǹ��������������ت������ܪ�������������������"};
const unsigned char PY_mb_gei[] = {"��"};
const unsigned char PY_mb_gen[] = {"����بݢ����"};
const unsigned char PY_mb_geng[] = {"�����������������������"};
const unsigned char PY_mb_gong[] = {"�������������칮�����������������������"};
const unsigned char PY_mb_gou[] = {"�������乷������������ڸ������������������"};
const unsigned char PY_mb_gu[] = {"�Ĺ̹Źǹʹ˹ɹȹ��͹¹üֹ�������������ڬ����������������������������������������"};
const unsigned char PY_mb_gua[] = {"�ҹι����йѹ���ڴ�������"};
const unsigned char PY_mb_guai[] = {"�ֹԹ���"};
const unsigned char PY_mb_guan[] = {"�عܹ۹ٹ��߹ڹݹ޹�����ݸ������������"};
const unsigned char PY_mb_guang[] = {"������������"};
const unsigned char PY_mb_gui[] = {"��������������������Ȳ���������������������"};
const unsigned char PY_mb_gun[] = {"�������������"};
const unsigned char PY_mb_guo[] = {"�����������й����������������������"};
const unsigned char PY_mb_h[] = {"�ͻ��λƺ����ػ��身���»��úϻ���ܻغź����򺣻����"};
const unsigned char PY_mb_ha[] = {"����Ϻ��"};
const unsigned char PY_mb_hai[] = {"�����������������ٿ�������"};
const unsigned char PY_mb_han[] = {"����������������������������������������������������������"};
const unsigned char PY_mb_hang[] = {"�к����������������"};
const unsigned char PY_mb_hao[] = {"�úź��ĺ��ºƸ亾������޶����������"};
const unsigned char PY_mb_he[] = {"�ͺϺӺκ˺պɺֺȺغǺ��źкʺѺҺԺ�ڭ�������������������"};
const unsigned char PY_mb_hei[] = {"�ں���"};
const unsigned char PY_mb_hen[] = {"�ܺݺۺ�"};
const unsigned char PY_mb_heng[] = {"����ߺ�޿������"};
const unsigned char PY_mb_hong[] = {"��������������ڧݦޮް����"};
const unsigned char PY_mb_hou[] = {"���������ܩ��������������"};
const unsigned char PY_mb_hu[] = {"�ͻ����������������������˺�Ϸ��������������������������������������������������������"};
const unsigned char PY_mb_hua[] = {"����������������������������"};
const unsigned char PY_mb_huai[] = {"��������������"};
const unsigned char PY_mb_huan[] = {"�������������û�������������ۨۼ��ߧ������������������"};
const unsigned char PY_mb_huang[] = {"�ƻɻĻʻŻȻǻ˻̻ͻλϻл��������������������"};
const unsigned char PY_mb_hui[] = {"��ػһӻԻ�ٻۻֻ�ݻ����黲�׻ڻܻ޻߻����ڶ����ޥ�������������������������"};
const unsigned char PY_mb_hun[] = {"�������ڻ������"};
const unsigned char PY_mb_huo[] = {"�ͻ���������������޽߫������������"};
const unsigned char PY_mb_j[] = {"���ֽ�������������ͽ��Ӿ��ҽ�׼������ϻ����ӽ̼��ż�"};
const unsigned char PY_mb_ji[] = {"������������������Ǽ���ϵ��Ƽ����ʼ����ü����̼����ȼͼļ����������漪�����������������������������������������¼żɼ˽�آؽ����٥��ڵ��ܸ������ު��ߴ����������������������������������������������������������"};
const unsigned char PY_mb_jia[] = {"�ӼҼܼۼ׼мټؼּڼݼμϼѼԼռ���Ю��٤ۣ�������������������������������"};
const unsigned char PY_mb_jian[] = {"�������������������轥������������߼��ǳ����뽢�����������������������������������������������������������������������"};
const unsigned char PY_mb_jiang[] = {"������ǿ������������������������������������"};
const unsigned char PY_mb_jiao[] = {"�Ͻ̽��ǽнŽ�������У�����ͽ��½ѽ����������ýĽƽȽɽʽ˽�ٮ��ܴ����������������������"};
const unsigned char PY_mb_jie[] = {"��׽�ӽڼҽ�ؽ��۽�ֽҽ�߽ܽԽսٽ۽ݽ޽�����뿬��ڦڵ���������������������"};
const unsigned char PY_mb_jin[] = {"���������񾡽������������������������ݣ����������������������"};
const unsigned char PY_mb_jing[] = {"��������������������������������������������������������ݼ������������������"};
const unsigned char PY_mb_jiong[] = {"����������"};
const unsigned char PY_mb_jiu[] = {"�;žɾ��þȾƾ������¾ľǾʾ˾̾�����������������"};
const unsigned char PY_mb_ju[] = {"�߾ݾ־ٳ���۾�޾Ӿ��ؾܹ�۾ϾоѾҾԾվ׾ھ������ڪ��������������������������������������������"};
const unsigned char PY_mb_juan[] = {"��������۲�������������"};
const unsigned char PY_mb_jue[] = {"�������ǽž������������������ާ�������������������������޶�"};
const unsigned char PY_mb_jun[] = {"�������������������������������"};
const unsigned char PY_mb_k[] = {"�ɿ�����˿����ܿƿڿտ�쿼�̿����������󿹿�"};
const unsigned char PY_mb_ka[] = {"��������������"};
const unsigned char PY_mb_kai[] = {"�������������������������"};
const unsigned char PY_mb_kan[] = {"��������������Ƕ٩ݨ������"};
const unsigned char PY_mb_kang[] = {"��������������������"};
const unsigned char PY_mb_kao[] = {"����������������"};
const unsigned char PY_mb_ke[] = {"�ɿ˿ƿ̿Ϳǿſÿºǿ����Ŀȿʿ����������������������������"};
const unsigned char PY_mb_ken[] = {"�Ͽпѿ���"};
const unsigned char PY_mb_keng[] = {"�ӿ��"};
const unsigned char PY_mb_kong[] = {"�׿տؿ�������"};
const unsigned char PY_mb_kou[] = {"�ڿۿٿ���ޢߵ����"};
const unsigned char PY_mb_ku[] = {"���ݿ�޿߿���ܥ����"};
const unsigned char PY_mb_kua[] = {"������٨"};
const unsigned char PY_mb_kuai[] = {"��������ۦ�������"};
const unsigned char PY_mb_kuan[] = {"������"};
const unsigned char PY_mb_kuang[] = {"�����������ڲڿ������������"};
const unsigned char PY_mb_kui[] = {"����������������������ظ���������������������������"};
const unsigned char PY_mb_kun[] = {"��������������������"};
const unsigned char PY_mb_kuo[] = {"��������������"};
const unsigned char PY_mb_l[] = {"������������³��������¬������½����¹����·������������������������������"};
const unsigned char PY_mb_la[] = {"���������������������������"};
const unsigned char PY_mb_lai[] = {"���������������������"};
const unsigned char PY_mb_lan[] = {"����������������������������������������"};
const unsigned char PY_mb_lang[] = {"��������������ݹ����������"};
const unsigned char PY_mb_lao[] = {"���������������������������������������"};
const unsigned char PY_mb_le[] = {"����������߷����"};
const unsigned char PY_mb_lei[] = {"��������������������������ڳ������������"};
const unsigned char PY_mb_leng[] = {"������ܨ�"};
const unsigned char PY_mb_li[] = {"��������������������������������������������������������������������تٳٵ۪����ݰ��޼߿����������������������������������������������������������"};
const unsigned char PY_mb_lia[] = {"��"};
const unsigned char PY_mb_lian[] = {"����������������������������������������������������"};
const unsigned char PY_mb_liang[] = {"����������������������ܮݹ�������"};
const unsigned char PY_mb_liao[] = {"��������������������������ޤ������������"};
const unsigned char PY_mb_lie[] = {"�������������������������"};
const unsigned char PY_mb_lin[] = {"���������������������������������������������"};
const unsigned char PY_mb_ling[] = {"��������������������������������۹����������������������"};
const unsigned char PY_mb_liu[] = {"������������½����������µ����������������"};
const unsigned char PY_mb_lo[] = {"��"};
const unsigned char PY_mb_long[] = {"��¢Ū��¡������£¤���������������"};
const unsigned char PY_mb_lou[] = {"©¶¥¦§¨ª�����������������"};
const unsigned char PY_mb_lu[] = {"·¼½¯¶«¬­®°±²³´µ¸¹º»¾��ߣ��������������������������������������"};
const unsigned char PY_mb_luan[] = {"��������������������"};
const unsigned char PY_mb_lue[] = {"�����"};
const unsigned char PY_mb_lun[] = {"����������������"};
const unsigned char PY_mb_luo[] = {"�����������߿������������������������������������������"};
const unsigned char PY_mb_lv[] = {"��������¿�������������������������������"};
const unsigned char PY_mb_m[] = {"����������÷������ëé��Īû����ôÿ����ĥ������ĶĿ߼"};
const unsigned char PY_mb_ma[] = {"��ô������������ĦĨ������������"};
const unsigned char PY_mb_mai[] = {"������������۽ݤ��"};
const unsigned char PY_mb_man[] = {"����������������áܬ��������������"};
const unsigned char PY_mb_mang[] = {"æâäãåç��������"};
const unsigned char PY_mb_mao[] = {"ëìðòóñèéêíîï��������������������"};
const unsigned char PY_mb_me[] = {"ô��"};
const unsigned char PY_mb_mei[] = {"ûÿ��úùø÷��üõöýþ����������ݮ���������������"};
const unsigned char PY_mb_men[] = {"��������������"};
const unsigned char PY_mb_meng[] = {"��������������å������ޫ��������������"};
const unsigned char PY_mb_mi[] = {"��������������������������������������������������������"};
const unsigned char PY_mb_mian[] = {"����������������������������"};
const unsigned char PY_mb_miao[] = {"�������������������������������"};
const unsigned char PY_mb_mie[] = {"����ؿ�����"};
const unsigned char PY_mb_min[] = {"�������������������������������"};
const unsigned char PY_mb_ming[] = {"��������������ڤ���������"};
const unsigned char PY_mb_miu[] = {"����"};
const unsigned char PY_mb_mo[] = {"��ĥû��ĩģĤ��īĦ��ĪĨðĬ�Ѻ�ġĢħĭĮįİ�����������������������"};
const unsigned char PY_mb_mou[] = {"ĳıĲٰ������������"};
const unsigned char PY_mb_mu[] = {"ĶĿľĸĹģĻ��ķ����ĲĴĵĺļĽ�������������"};
const unsigned char PY_mb_n[] = {"����������ũ��������Ů��������ţ����ճ������"};
const unsigned char PY_mb_na[] = {"��������������������������"};
const unsigned char PY_mb_nai[] = {"������������ؾܵ����"};
const unsigned char PY_mb_nan[] = {"������������������"};
const unsigned char PY_mb_nang[] = {"��߭������"};
const unsigned char PY_mb_nao[] = {"����Ŭ������ث������������"};
const unsigned char PY_mb_ne[] = {"������ګ"};
const unsigned char PY_mb_nei[] = {"��������"};
const unsigned char PY_mb_nen[] = {"����"};
const unsigned char PY_mb_neng[] = {"��"};
const unsigned char PY_mb_ni[] = {"��������������������������٣������������������"};
const unsigned char PY_mb_nian[] = {"����ճ������������إ���������"};
const unsigned char PY_mb_niang[] = {"����"};
const unsigned char PY_mb_niao[] = {"��������������"};
const unsigned char PY_mb_nie[] = {"��������������ؿ����������"};
const unsigned char PY_mb_nin[] = {"����"};
const unsigned char PY_mb_ning[] = {"����š����Ţ���������"};
const unsigned char PY_mb_niu[] = {"ţŤťŦ�������"};
const unsigned char PY_mb_nong[] = {"ũŪŨŧٯ��"};
const unsigned char PY_mb_nou[] = {"��"};
const unsigned char PY_mb_nu[] = {"Ŭūŭ��������"};
const unsigned char PY_mb_nuan[] = {"ů"};
const unsigned char PY_mb_nuo[] = {"ŵ��ŲųŴ�������"};
const unsigned char PY_mb_nv[] = {"Ů������"};
const unsigned char PY_mb_nve[] = {"Űű"};
const unsigned char PY_mb_o[] = {"Ŷ���ŷżŸŹźŻ����"};
const unsigned char PY_mb_ou[] = {"��ŷżŸŹźŻŽک�����"};
const unsigned char PY_mb_p[] = {"��ƽ��������������Ƭ������ƷƤ����ƫ��������ƶ����"};
const unsigned char PY_mb_pa[] = {"���������ǰ�žſ����������"};
const unsigned char PY_mb_pai[] = {"����������������ٽ��"};
const unsigned char PY_mb_pan[] = {"�����ѷ��������������������������"};
const unsigned char PY_mb_pang[] = {"�԰�����������������"};
const unsigned char PY_mb_pao[] = {"������������������������"};
const unsigned char PY_mb_pei[] = {"�����������������������������"};
const unsigned char PY_mb_pen[] = {"������"};
const unsigned char PY_mb_peng[] = {"����������������������������ܡ�����"};
const unsigned char PY_mb_pi[] = {"��Ƥ�������Ƣƣ������������ơƥƦƧƨƩا��������ۯ����ܱ��ߨ����������������������������"};
const unsigned char PY_mb_pian[] = {"Ƭƫ��ƪƭ����������������"};
const unsigned char PY_mb_piao[] = {"ƱƯ��Ʈư��ݳ������������"};
const unsigned char PY_mb_pie[] = {"ƲƳد���"};
const unsigned char PY_mb_pin[] = {"ƷƶƵƴƻƸ���������"};
const unsigned char PY_mb_ping[] = {"ƽ��ƿƾƻ��ƹƺƼ��ٷ�����ڢ"};
const unsigned char PY_mb_po[] = {"�������÷��Ӳ�������������۶��������������"};
const unsigned char PY_mb_pou[] = {"������"};
const unsigned char PY_mb_pu[] = {"���������̱������ѱ��������������������������������"};
const unsigned char PY_mb_q[] = {"��ǰ��Ǯ����������������ȥ��ȫ��Ⱥ����ǿ��ȡ������ȷȨ����"};
const unsigned char PY_mb_qi[] = {"���������������������������������������������������������������������������ؽ��ٹ��ܻ����ݽ����ޭ���������������������������������������������"};
const unsigned char PY_mb_qia[] = {"��ǡ��Ǣ٤������"};
const unsigned char PY_mb_qian[] = {"ǰǧǮǳǩǨǦǱǣǯ��ǴǤǥǪǫǬǭǲǵǶǷǸٻ����ܷ����ݡ�������������������������"};
const unsigned char PY_mb_qiang[] = {"ǿ��ǹ��ǽǻǺǼǾ����������������������"};
const unsigned char PY_mb_qiao[] = {"���ƿ������ǽ���������������������ȸ��ڽ�����������������"};
const unsigned char PY_mb_qie[] = {"������������٤ۧ���������"};
const unsigned char PY_mb_qin[] = {"����������������������������������������������"};
const unsigned char PY_mb_qing[] = {"��������������������������������������������������"};
const unsigned char PY_mb_qiong[] = {"�����������������"};
const unsigned char PY_mb_qiu[] = {"������������������ٴ��������������������������"};
const unsigned char PY_mb_qu[] = {"ȥ��ȡ������������Ȥ����Ȣȣ��ڰ۾��ޡ޾��������������������������"};
const unsigned char PY_mb_quan[] = {"ȫȨȦȰȪȩȧȬȭȮȯڹ���������������"};
const unsigned char PY_mb_que[] = {"ȷȴȱȲȳȵȶȸ������"};
const unsigned char PY_mb_qun[] = {"Ⱥȹ��"};
const unsigned char PY_mb_r[] = {"������������������Ȼ��������ȼȾ������������"};
const unsigned char PY_mb_ran[] = {"ȻȼȾȽ������"};
const unsigned char PY_mb_rang[] = {"������ȿ�����"};
const unsigned char PY_mb_rao[] = {"�����������"};
const unsigned char PY_mb_re[] = {"������"};
const unsigned char PY_mb_ren[] = {"���������������������������������"};
const unsigned char PY_mb_reng[] = {"����"};
const unsigned char PY_mb_ri[] = {"��"};
const unsigned char PY_mb_rong[] = {"������������������������������"};
const unsigned char PY_mb_rou[] = {"������������"};
const unsigned char PY_mb_ru[] = {"����������������������޸������������"};
const unsigned char PY_mb_ruan[] = {"������"};
const unsigned char PY_mb_rui[] = {"��������ި����"};
const unsigned char PY_mb_run[] = {"����"};
const unsigned char PY_mb_ruo[] = {"����ټ��"};
const unsigned char PY_mb_s[] = {"����ʱ����������ʩ������ʦ˧ɳ��ʯʷ��˵����ˮ����ʮʹ������ʵ��ʽɽ��"};
const unsigned char PY_mb_sa[] = {"������ئ�������"};
const unsigned char PY_mb_sai[] = {"��˼��������"};
const unsigned char PY_mb_san[] = {"��ɢ��ɡ�����"};
const unsigned char PY_mb_sang[] = {"ɣɥɤ�����"};
const unsigned char PY_mb_sao[] = {"ɨ��ɦɧɩܣ����������"};
const unsigned char PY_mb_se[] = {"ɫ��ɪɬ����"};
const unsigned char PY_mb_sen[] = {"ɭ"};
const unsigned char PY_mb_seng[] = {"ɮ"};
const unsigned char PY_mb_sh[] = {"����ɽɾ����������������ʦʼ��������������˫ˮ˭˯˰˳"};
const unsigned char PY_mb_sha[] = {"ɳɱɰɶɴɯɲɵɷɼ������������������"};
const unsigned char PY_mb_shai[] = {"ɫɸɹ"};
const unsigned char PY_mb_shan[] = {"ɽ������ɺ���µ�ɻɼɾɿ������������դ��ڨ۷��������������������������"};
const unsigned char PY_mb_shang[] = {"���������������������������"};
const unsigned char PY_mb_shao[] = {"��������������������������ۿ�����������"};
const unsigned char PY_mb_she[] = {"������������������������������ߡ�������"};
const unsigned char PY_mb_shen[] = {"������ʲ���������������������������ڷ��ݷ�������������"};
const unsigned char PY_mb_sheng[] = {"��ʤ��ʡ��ʢ��ʣʥ����������������"};
const unsigned char PY_mb_shi[] = {"��ʱʮʹ��ʵʽʶ����ʯʲʾ��ʷʦʼʩʿ��ʪ��ʳʧ��������ʴʫ��ֳʰ��ʻ��ʨʬʭʸʺ��������������������������ݪ��߱������������������������"};
const unsigned char PY_mb_shou[] = {"���������������������������"};
const unsigned char PY_mb_shu[] = {"����������������������������������������������������������������ˡ��حٿ������������������"};
const unsigned char PY_mb_shua[] = {"ˢˣ�"};
const unsigned char PY_mb_shuai[] = {"˥˧ˤ˦�"};
const unsigned char PY_mb_shuan[] = {"˨˩����"};
const unsigned char PY_mb_shuang[] = {"˫˪ˬ����"};
const unsigned char PY_mb_shui[] = {"ˮ˵˭˯˰"};
const unsigned char PY_mb_shun[] = {"˳˱˲˴"};
const unsigned char PY_mb_shuo[] = {"˵��˶˷˸����������"};
const unsigned char PY_mb_si[] = {"��˼��˹˿��˾��ʳ˽��˺˻������������ٹ����������������������������"};
const unsigned char PY_mb_song[] = {"����������������ڡݿ����������"};
const unsigned char PY_mb_sou[] = {"����������޴�������������"};
const unsigned char PY_mb_su[] = {"�����������������������������������������"};
const unsigned char PY_mb_suan[] = {"�������"};
const unsigned char PY_mb_sui[] = {"��������������������������ݴ��������"};
const unsigned char PY_mb_sun[] = {"������ݥ������"};
const unsigned char PY_mb_suo[] = {"��������ɯ�����������������������"};
const unsigned char PY_mb_t[] = {"��ͬ����̷����ͯͿ��١������ͼ��ͷͨ��������ͳ̨��̫����"};
const unsigned char PY_mb_ta[] = {"��������̤��̡̢̣��������������������"};
const unsigned char PY_mb_tai[] = {"̨̧̫̬̥̩̦̪̭ۢ޷߾������������"};
const unsigned char PY_mb_tan[] = {"̸̼̽̿��̴̵̶̷̹̰̲̮̯̱̳̺̻̾۰����������"};
const unsigned char PY_mb_tang[] = {"����������������������������������������������"};
const unsigned char PY_mb_tao[] = {"����������������������ػ߶��������"};
const unsigned char PY_mb_te[] = {"��߯�����"};
const unsigned char PY_mb_teng[] = {"����������"};
const unsigned char PY_mb_ti[] = {"�����������������������������������������������"};
const unsigned char PY_mb_tian[] = {"��������������������������"};
const unsigned char PY_mb_tiao[] = {"������������٬�����������������"};
const unsigned char PY_mb_tie[] = {"����������"};
const unsigned char PY_mb_ting[] = {"��ͣͥͦ͢����ͤͧ͡���������������"};
const unsigned char PY_mb_tong[] = {"ͬͨͳͭʹͲͯͰ����ͩͪͫͮͱ١���������������"};
const unsigned char PY_mb_tou[] = {"ͷͶ͸͵����"};
const unsigned char PY_mb_tu[] = {"ͼ��ͻ;ͽ͹Ϳ������ͺܢݱ������"};
const unsigned char PY_mb_tuan[] = {"���������ͼ��"};
const unsigned char PY_mb_tui[] = {"������������߯��"};
const unsigned char PY_mb_tun[] = {"�����������������"};
const unsigned char PY_mb_tuo[] = {"���������ֶ���������������ر٢������������������������"};
const unsigned char PY_mb_w[] = {"Ϊ����������κ��Τ������������������λ��ί��������������Χ����"};
const unsigned char PY_mb_wa[] = {"���ڰ����������������"};
const unsigned char PY_mb_wai[] = {"������"};
const unsigned char PY_mb_wan[] = {"����������������������������������������ܹݸ������������"};
const unsigned char PY_mb_wang[] = {"����������������â�������������"};
const unsigned char PY_mb_wei[] = {"ΪλίΧάΨ��΢ΰδ��Σβνιζθκα��ΥΤηγΡΦΩΫέήεμξο������������ޱ���������������������������������"};
const unsigned char PY_mb_wen[] = {"�������������������������������"};
const unsigned char PY_mb_weng[] = {"��������޳"};
const unsigned char PY_mb_wo[] = {"��������������������ݫ���������"};
const unsigned char PY_mb_wu[] = {"�������������������������������������������������������������أ����������������������������������������������"};
const unsigned char PY_mb_x[] = {"ѧ������������ФѦ�ܽ���ϰ��С������Щ��������ϵϯ������������"};
const unsigned char PY_mb_xi[] = {"ϵϯ��ϰϸ����ϲϴϳϡϷ϶ϣϢϮ��ϩ��ϤϧϪ����������������ϥϦϨϫϬϭϱ������ۭ��ݾ������������������������������������������������������������"};
const unsigned char PY_mb_xia[] = {"��������ϼ��ϹϺϻϽϾϿ���������������������"};
const unsigned char PY_mb_xian[] = {"���������޼���������������ϴ����ϳ����������������������������ݲ޺����������������������������"};
const unsigned char PY_mb_xiang[] = {"���������������罵������������������������ܼ��������������"};
const unsigned char PY_mb_xiao[] = {"С����ЧЦУ������ФТ������������Х�����������������������"};
const unsigned char PY_mb_xie[] = {"Щ��дблЭеҶѪжмЬЪавзйк��ШЫЮЯги��������ޯߢ�����������������"};
const unsigned char PY_mb_xin[] = {"������по��Ѱ��н����ضܰ������"};
const unsigned char PY_mb_xing[] = {"������������ʡ������������������������ߩ����"};
const unsigned char PY_mb_xiong[] = {"��������������ܺ"};
const unsigned char PY_mb_xiu[] = {"�������������޳���������������������"};
const unsigned char PY_mb_xu[] = {"����������������������������������������ڼ����ޣ��������������������"};
const unsigned char PY_mb_xuan[] = {"ѡ��������ȯ����ѢѣѤ������������������������������"};
const unsigned char PY_mb_xue[] = {"ѧѪ��ѩѨѥѦ����������"};
const unsigned char PY_mb_xun[] = {"ѵѮѸѶѰѭѲ�翣ѫѬѯѱѳѴѷ��ۨ����ަ޹�����������������"};
const unsigned char PY_mb_y[] = {"һ����������Ҧ��Ԭ���������Ҷ��������Ҫ����Ҳ��������ѹԱ��������ԭ����"};
const unsigned char PY_mb_ya[] = {"ѹ��ѽ��ѿ����ѼƥѺѻѾ�������������������������������"};
const unsigned char PY_mb_yan[] = {"����������������������������������Ǧ����������������������������������������ٲ��������۱۳ܾ�����������������������������������������"};
const unsigned char PY_mb_yang[] = {"�������������������������������������������������"};
const unsigned char PY_mb_yao[] = {"ҪҩԼҡ��ҧ��ҫ�ս�ű����ҢңҤҥҦҨԿزسߺ������������������������"};
const unsigned char PY_mb_ye[] = {"ҲҵҳҶҺҹҰүұа����ҬҭҮҴҷҸק��������������"};
const unsigned char PY_mb_yi[] = {"һ����������ҽ��������������������������������������������ҼҾҿ��������������������������������������������������٫ڱ����ܲ����޲������߽߮�����������������������������������������������������������������"};
const unsigned char PY_mb_yin[] = {"������ӡ��������������������������ط۴����ܧ���������������������"};
const unsigned char PY_mb_ying[] = {"ӦӰӲӪӢӳӭӣӤӥӧӨөӫӬӮӯӱ��۫����ݺ��������������������������"};
const unsigned char PY_mb_yo[] = {"��Ӵ�"};
const unsigned char PY_mb_yong[] = {"������ӵӿӼӹӶӷӸӺӻӽӾ��ٸ��ܭ�����������"};
const unsigned char PY_mb_you[] = {"��������������������������������������������٧ݬݯݵ��������������������������������"};
const unsigned char PY_mb_yu[] = {"����������������Ԥ����������������������εξ������������������������������������������ԡԢԣԥԦ��خع��ٶ�����������������������������������������������������������������������������������"};
const unsigned char PY_mb_yuan[] = {"ԱԭԲԴԪԶԸԺԵԮ԰ԹԧԨԩԫԬԯԳԷ��ܫܾ��������������������"};
const unsigned char PY_mb_yue[] = {"��˵ԽԼ��ԾԻ��Կ����������������"};
const unsigned char PY_mb_yun[] = {"��Ա���Ⱦ�������������������۩ܿ�������������"};
const unsigned char PY_mb_z[] = {"����������������֣����������ף��ׯ��ղ�����չ��ս����֮��������ֻ������ָ"};
const unsigned char PY_mb_za[] = {"����������զ��"};
const unsigned char PY_mb_zai[] = {"��������������������"};
const unsigned char PY_mb_zan[] = {"���������������������"};
const unsigned char PY_mb_zang[] = {"�������������"};
const unsigned char PY_mb_zao[] = {"������������������������������"};
const unsigned char PY_mb_ze[] = {"���������զ�����������������"};
const unsigned char PY_mb_zei[] = {"��"};
const unsigned char PY_mb_zen[] = {"����"};
const unsigned char PY_mb_zeng[] = {"����������������"};
const unsigned char PY_mb_zh[] = {"��ըժծսչ������������������������֮����ֻ��������סץתר׫׬װ״׼"};
const unsigned char PY_mb_zha[] = {"����ըբա����������գդեզէթ����߸��������������"};
const unsigned char PY_mb_zhai[] = {"կ����ժխի�Լ�լծ���"};
const unsigned char PY_mb_zhan[] = {"սչվռճ��հձղմյնշոպջտ�������"};
const unsigned char PY_mb_zhang[] = {"����������������������������������۵��������"};
const unsigned char PY_mb_zhao[] = {"�������г�����צ�����׳�����گ��������"};
const unsigned char PY_mb_zhe[] = {"������������������������ߡ��������������"};
const unsigned char PY_mb_zhen[] = {"������������������������������������������������������������"};
const unsigned char PY_mb_zheng[] = {"��������֤����֢��֣����������֡ں��������"};
const unsigned char PY_mb_zhi[] = {"֮����ֻ��ֱָֹ֧֯����־ֵ֪ʶְֲִֽ��ֳ֦֬����֫��ַ��ֶֺּֿ֥֭֨֩��������������ش��ۤ���������������������������������������������������������������"};
const unsigned char PY_mb_zhong[] = {"����������������������ڣ���������"};
const unsigned char PY_mb_zhou[] = {"����������������������������ݧ����������������"};
const unsigned char PY_mb_zhu[] = {"��ע��ס������������������פ��ף��������������������������٪ۥ����������������������������������"};
const unsigned char PY_mb_zhua[] = {"ץצ��"};
const unsigned char PY_mb_zhuai[] = {"תҷק��"};
const unsigned char PY_mb_zhuan[] = {"תר��ש׫׬׭�������"};
const unsigned char PY_mb_zhuang[] = {"װ״׳ׯײ��׮ױ����"};
const unsigned char PY_mb_zhui[] = {"׷׶׵׸׹׺�����"};
const unsigned char PY_mb_zhun[] = {"׼��׻���"};
const unsigned char PY_mb_zhuo[] = {"����׽����׾׿������������پ��ߪ��������������"};
const unsigned char PY_mb_zi[] = {"����������������֨���������������������������������������������������������"};
const unsigned char PY_mb_zong[] = {"����������������������"};
const unsigned char PY_mb_zou[] = {"������������۸����"};
const unsigned char PY_mb_zu[] = {"����������������������"};
const unsigned char PY_mb_zuan[] = {"��׬��߬����"};
const unsigned char PY_mb_zui[] = {"������Ѿ���ީ"};
const unsigned char PY_mb_zun[] = {"����ߤ����"};
const unsigned char PY_mb_zuo[] = {"������������������������������������"};

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

// �Ƚ������ַ�����ƥ�����
//����ֵ:0xff,��ʾ��ȫƥ��.
//        ����,ƥ����ַ���
u8 str_match(u8 *str1, u8 *str2)
{
    u8 i = 0;
    while (1)
    {
        if (*str1 != *str2)
            break; //����ƥ��
        if (*str1 == '\0')
        {
            i = 0XFF;
            break;
        } //��ȫƥ��
        i++;
        str1++;
        str2++;
    }
    return i; //�����ַ������
}

//��ȡƥ���ƴ�����
//*strin,������ַ���,����:"726"
//**matchlist,�����ƥ���.
//����ֵ:[7],0,��ʾ��ȫƥ�䣻1����ʾ����ƥ�䣨����û����ȫƥ���ʱ��Ż���֣�
// [6:0],��ȫƥ���ʱ�򣬱�ʾ��ȫƥ���ƴ������
//            ����ƥ���ʱ�򣬱�ʾ��Чƥ���λ��
u8 get_matched_pymb(u8 *strin, T9PY_IDX **matchlist)
{
    T9PY_IDX *bestmatch; //���ƥ��
    u16 pyindex_len;
    u16 i;
    u8 temp, mcnt = 0, bmcnt = 0;
    bestmatch = (T9PY_IDX *)&t9PY_index[0];                   //Ĭ��Ϊa ��ƥ��
    pyindex_len = sizeof(t9PY_index) / sizeof(t9PY_index[0]); //�õ�py �������Ĵ�С.
    for (i = 0; i < pyindex_len; i++)
    {
        temp = str_match(strin, (u8 *)t9PY_index[i].PY);
        if (temp)
        {
            if (temp == 0XFF)
            {
                matchlist[mcnt++] = (T9PY_IDX *)&t9PY_index[i];
            }
            else if (temp > bmcnt) //�����ƥ��
            {
                bmcnt = temp;
                bestmatch = (T9PY_IDX *)&t9PY_index[i]; //��õ�ƥ��.
            }
        }
    }
    if (mcnt == 0 && bmcnt) //û����ȫƥ��Ľ��,�����в���ƥ��Ľ��
    {
        matchlist[0] = bestmatch;
        mcnt = bmcnt | 0X80; //���ز���ƥ�����Чλ��
    }
    return mcnt; //����ƥ��ĸ���
}

//�õ�ƴ�����.
//str:�����ַ���
//����ֵ:ƥ�����.
u8 get_pymb(u8 *str, u8 *mb)
{
    u8 mcnt = get_matched_pymb(str, t9);
    u8 i, l, mbl;
    l = 0;
    for (i = 0; i < (mcnt & 0x7F); i++)
    {
        mbl = strlen((const char *)(t9[i]->MB));
        if ((l + mbl) > 126) //��������128�ֽ�
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

unsigned char hzdispwin = 7; //ÿ��������ʾ����

typedef struct
{
    unsigned char py[6];  //ƴ�� (6BYTE)
    unsigned short begin; //��Ӧ�������ʼ��� (2BYTE)
} PYCODETABLE;            // (8BYTE)

int pyfd;                         //ƴ��������
unsigned short pysum;             //ƴ����ϵĸ���
unsigned short hzsum;             //����еĺ��ָ���
unsigned short pyidx[26];         //��26 ��Ӣ����ĸ��ͷ��ƴ��������
unsigned short pyseek;            //ƴ�����ҵĿ�ʼλ�� ,�ƶ�ָ����
unsigned short firsthz;           //��һ����������λ�ã�Ϊ����hzseek �������һ��ƴ���ĺ��ָ���
unsigned short hzseek;            //���ֲ��ҵĿ�ʼλ��,curhz �ƶ�ָ����
unsigned short curhz = 0;         //��ǰ��ʾ���ֵĺţ������ƶ�����Ϊhzdispwin
unsigned short maxcurhz;          //��ǰƴ�����������
unsigned char hzstring[20];       //���溺�����룬��СӦΪhzdispwin*2+1//������ʾ����MAX=10 ��
unsigned long baseaddr = 0x72790; //�����ֿ�оƬ��ʵ�ʵ�ַ�����޸ģ��������ֲᣩ

void pyInit(void)
{
    short i;
    unsigned long addr = 0x0000;
    PYCODETABLE py;
    unsigned char bytmp;
    addr = 0x0000 + baseaddr; //pysum ƴ�������(2BYTE)
    //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);
    pysum = bytmp;
    pysum = pysum << 8;
    //SpiFlash_Read(&bytmp,addr,1);
    read_font_addr(addr, &bytmp, 1);

    pysum = pysum + bytmp;
    addr = 0x0002 + baseaddr; //hzsum ���뺺�������(2BYTE)
                              //SpiFlash_Read(&bytmp,addr++,1);
    read_font_addr(addr++, &bytmp, 1);

    hzsum = bytmp;
    hzsum = hzsum << 8;
    //SpiFlash_Read(&bytmp,addr,1);
    read_font_addr(addr, &bytmp, 1);

    hzsum = hzsum + bytmp;
    addr = 0x0004 + baseaddr; //��26 ��Ӣ����ĸ��ͷ��ƴ��������(26*2=52BYTE)
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

    pyseek = pyidx[0];        //ƴ�����ҵ���Կ�ʼ��ַ ,�ƶ�ָ����
    addr = pyseek + baseaddr; //��ʼ��ʱ��������һ��ĸ��a����ַ(��ʼֵ)
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
        //ˢ����ʾ���뷨��
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
                typewriting_text_gbk_display(size_12, display_left, i * 18, 95, (u8 *)Temp, 1, COLOR_BLUE, COLOR_WIGHT); //��ʾ����

                if ((Pinyin.SelIndex >= 0) && (i == (Pinyin.SelIndex % PY_PAGE_MAX_NUM)))
                {
                    //UI_RcichTextAppendText(&Text_Code, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), FONT_GB2312, COLOR_RED, 2);
                    typewriting_text_gbk_display(size_12, display_left, i * 18 + 6, 95, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), 2, COLOR_RED, COLOR_WIGHT);
                }
                else
                {
                    //UI_RcichTextAppendText(&Text_Code, (u8 *)(Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2), FONT_GB2312, COLOR_BLACK, 2);
                    typewriting_text_gbk_display(size_12, display_left, i * 18 + 6, 95, (u8 *)Pinyin.DataBuf + (Pinyin.SelPage * PY_PAGE_MAX_NUM + i) * 2, 2, COLOR_BLUE, COLOR_WIGHT); //��ʾ�������
                }
            }
        }
				else
				{
					//���ƴ�����뷨��ɾ�������һ���������ѡ�ֵ�����
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
    const char MsgTable[4][5] = {"123 ", "abc ", "����", "����"};
#else
    const char MsgTable[4][5] = {"123 ", "abc ", "����", "����"};
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

        //��ֵˢ��ʱ��һ��
        if (keyboard_number_old != get_keyboard_number())
        {
            keyboard_number_old = get_keyboard_number();

            if (keyboard_number_old != 0x00)
            {
                //�������ƴ�����뷨�����ƴ���������ѡ
                if (KeyBoard.state == KB_STATE_ON)
                {
                    if (KeyBoard.Type == KB_PINYIN)
                    {
                        //if((Key.Code>=K1)&&(Key.Code<=K9))
                        if (get_keyboard_number() >= 0x31 && get_keyboard_number() <= 0x39)
                        {
                            ui.page = UI_PAGE_PHONE_NEW_CONTACT_SON; //��ƴ���������뷨����
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
                                            //ˢ����ʾ���뷨��
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
                                ui.page = UI_PAGE_PHONE_NEW_CONTACT; //����ƴ���˳����뷨����

                                //�����ر�־λ
                                Pinyin_Init();
                            }
                            KeyBoard_InputChar();
                        }
                    }
                    //���ּ���
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
    case UI_OPT_FIRST_REFRESH: //��һ�ν���
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
    case UI_OPT_KEY_UP: //���л���Ŀ
        sel = (sel + 1) % 2;
        break;
    case UI_OPT_KEY_DOWN: //���л���Ŀ
        sel = (sel + 1) % 2;
        break;
    case UI_OPT_KEY_ENTER: //ȷ�ϱ�������
                           //ƴ������ģʽ
        if (KeyBoard.Type != KB_PINYIN)
        {
            //�����ر�־λ
            Pinyin_Init();
        }

        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex >= 0)
            {
                Pinyin_Select();
                //ѡ�к�������뷨����
            }
        }
        //�������ѡ��
        else if (Symble.Enable == true)
        {
            Symble_SetSelectSymble(&Symble);
        }
        else
        {
            KeyBoard.state = KB_STATE_OFF;
            if ((strlen((char *)new_caontact.name) > 0) && (strlen((char *)new_caontact.phone) > 0))
            {
                //������ϵ�˵�flash
                deposit_new_contact();
            }
            ui_phone(UI_OPT_PART_REFRESH);
        }
        break;
    case UI_OPT_KEY_BACK: //������һ��
        ui_phone(UI_OPT_PART_REFRESH);
        break;
    case UI_OPT_KEY_KEYBOARD:

        if (ui.page == UI_PAGE_PHONE_NEW_CONTACT || ui.page == UI_PAGE_PHONE_NEW_CONTACT_SON)
        {
            //ʵʱˢ��
            switch (sel)
            {
            case 0: //���������
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
                        if (InputChar[0] == '#') //�л����뷨
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
                        else if (InputChar[0] == '*') //ɾ��
                        {
                            u8 lenPinyinInBuf;
                            lenPinyinInBuf = strlen(Pinyin.InputBuf);
                            if ((KeyBoard.Type == KB_PINYIN) && (lenPinyinInBuf > 0))
                            {
                                if (Pinyin.SelIndex == -1)//���ƴ�����뷨���水�¼�����ѡ�񴰿�ɾ����ʾ�쳣�����⣨���ڴ�ѡ�ֽ��水ɾ��ʧЧ��
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

                //��ʾ���
                //        RichText.Cousor.LocX = 18 + strlen((char*)new_caontact.name) * 8;
                //        RichText.Cousor.LocY = 56;
                //        UI_ShowTextCousor(&RichText, 0);
                break;
            case 1: //����ں���
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

                //��ʾ���
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
        //ֻ��һ��
        if (sel != lastSel)
        {
            lastSel = sel;
            switch (sel)
            {
            case 0:
                //��ʾ���
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
                //��ʾ���
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
    case UI_OPT_FIRST_REFRESH: //��һ�ν���
        ui.page = UI_PAGE_PHONE_NEW_CONTACT_SON;
        break;
    case UI_OPT_KEY_UP: //���л���Ŀ
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
    case UI_OPT_KEY_DOWN: //���л���Ŀ
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
    case UI_OPT_KEY_ENTER: //ȷ�ϱ�������
        //ƴ������ģʽ
        if (Pinyin.Enable == true)
        {
            if (Pinyin.SelIndex >= 0)
            {
                Pinyin_Select();
            }
        }
        //�������ѡ��
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
            //ˢ����ʾ���뷨��
            lcd_api_clear(0, 74, COLOR_WIGHT, 160, 38);
        }
        ui.page = UI_PAGE_PHONE_NEW_CONTACT; //����ȷ�ϼ��󣬹�귵���ϼ�ҳ��

        break;
    case UI_OPT_KEY_BACK: //������һ��
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

//[0]:���±�־  [1]: ��ѡ��־ [2]�������ַ�
bool KeyBoard_GetInputChar(char *CharData, char *ReSelFlag)
{
    //�����ݸ���
    if (KeyBoard.InCharBuf[0])
    {
        KeyBoard.InCharBuf[0] = 0; //������ݸ��±�־
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
    // 1~ 9 �� ��עƴ����ĸ
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
    if (KeyCode != 0) //����ֵ=��ʱ��������⣬���=0ʱ��1�л�����abc������
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
            KeyBoard.InCharBuf[0] = 1; //����

            KeyBoard.InCharBuf[1] = RepeatFlag; //�Ƿ��ظ�

            KeyBoard.InCharBuf[2] = Temp; //ʵ�������ַ�
        }
        else
        {
            KeyBoard.InCharBuf[0] = 0; //δ����
        }
    }
}

char KeyBoard_KeyChar(u8 CharIndex)
{
    u8 KeyCode = 0;

    KeyCode = get_keyboard_number();

    //���ּ���
    if (KeyBoard.Type == KB_NUMBER)
    {
        //���ּ� 1~9
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

        if (KeyCode >= 0x31 && KeyCode <= 0x39) //ƴ������ & Ӣ�ļ���
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
            data_len = 26; //size_12�����ʾ26��Ӣ�Ļ�13������
        }
    }
    else if (size == size_16)
    {
        single_data_size = 8;
        if (data_len > 19)
        {
            data_len = 19; //size_16�����ʾ19��Ӣ�Ļ�9������
        }
    }
    else
    {
    }

    if (display_type == display_middle) //����
    {
        x = (160 - LocX - single_data_size * data_len) / 2;
    }
    else //���Һ��Զ���
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
            else //�����ˣ��Ҳ����ֿ�
            {
#ifdef JIANPUZHAI
                uin_dat16 = (gb2312_dat[i]) + (gb2312_dat[(i + 1)] << 8); //0x17��0x80�ϳ�Ϊ0x1780
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
