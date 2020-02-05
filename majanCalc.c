#include <stdio.h>
#include <stdlib.h>

void read ();
void readHai (int, char*);
void readNaki (int, char*);
void getInfo ();
void determin ();
void detHai (int);
void calc ();
void result ();
void disp (int*, int*);
void dispKokushi ();
int tensu (int*, int*);
void resetYaku ();
void printYaku (int*);
void copyMen ();
void copyYaku ();
int sumHan ();
int sumFu ();
void shuntsu (int);
void kotsu (int);
void janto (int);
void chi (int);
void pon (int);
void ankan (int);
void minkan (int);
void toitsu ();
void kokushi ();
int isShuntsu (int);
int isChi (int);
int isKotsu (int);
int isJanto (int);
int isPon (int);
int isMinkan (int);
int isAnkan (int);
int isNothing (int);
int shuntsuNum (int);
int chiNum (int);



int hai[40], naki[40], men1[40], men2[40], yaku[36];
// manzu:1~9,  pinzu:11~19,  so-zu:21~29,  tsu-hai:31~37
// men1[] = 1(shuntsu), 5(chi-)
// men2[] = 1(kotsu), 2(janto), 3(pon), 4(minkan), 5(ankan);
int bafu, jifu, agari, dora, rhc, richi, ippatsu, machi, menzen=1;

int atama=0, total=0;
int maxmen1[40], maxmen2[40], maxyaku[36], maxhan=0, maxfu=0;


int main (int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "./a.out [場風:東=1,南=2,西=3,北=4]\n");
        exit(0);
    }
    bafu = atoi(argv[1]);
    if (bafu<1 || 4<bafu) {
        fprintf(stderr, "Invalid number\n");
        exit(0);
    }
    bafu += 30;


    read();
    determin();
    result();

    return 1;
}


void read ()
{
    char str[20];
    int i;

    printf("門前の牌を入力してください\n");

    printf("萬子: ");
    readHai(0, str);

    printf("筒子: ");
    readHai(1, str);

    printf("索子: ");
    readHai(2, str);

    printf("字牌: ");
    readHai(3, str);

    if (total > 14) {
        fprintf(stderr, "error : 多牌です\n");
        exit(0);
    }
    if (total != 14) {
        printf("鳴きの牌を入力してください (暗槓は５つ)\n");
        menzen = 0;

        printf("萬子: ");
        readNaki(0, str);

        printf("筒子: ");
        readNaki(1, str);

        printf("索子: ");
        readNaki(2, str);

        printf("字牌: ");
        readNaki(3, str);

        for (i=1; i<38; i++) {
            if (isPon(i) || isChi(i) || isMinkan(i))
                break;
        }
        if (i==38)
            menzen = 1;
    }

    if (total != 14) {
        if (total < 14) {
            fprintf(stderr, "error : 少牌です\n");
            exit(0);
        } else {
            fprintf(stderr, "error : 多牌です\n");
            exit(0);
        }
    }
    if (hai[38] != 0 || hai[39] != 0) {
        fprintf(stderr, "東1, 南2, 西3, 北4, 白5, 發6,  中7\n");
        exit(0);
    }
}


void readHai (int kind, char *str)
{
    int i, num;
    fgets(str, 15, stdin);
    for (i=0; str[i] != '\n'; i++) {
        num = str[i]-'0';
        if (num<1 || 9<num) {
            fprintf(stderr, "Invalid Number\n");
            exit(0);
        }
        hai[kind*10 + num]++;
        total++;
    }
}


void readNaki (int kind, char *str)
{
    int i=0, n1, n2, n3, n4, n5;
    fgets(str, 20, stdin);
    while (str[i] != '\n' && i<20) {
        n1 = str[i]-'0';
        n2 = str[i+1]-'0';
        n3 = str[i+2]-'0';
        if (n1<1 || 9<n1) {
            fprintf(stderr, "Invalid Number\n");
            exit(0);
        }
        if (n1 != n2) {
            chi(10*kind+n1);
            i += 3;
        } else {
            if (n1 == n3) {
                n4 = str[i+3]-'0';
                if (n1 == n4) {
                    n5 = str[i+4]-'0';
                    if (n1 == n5) {
                        ankan(10*kind+n1);
                        i += 5;
                    } else {
                        minkan(10*kind+n1);
                        i += 4;
                    }
                } else {
                    pon(10*kind+n1);
                    i += 3;
                }
            }
        }
        total += 3;
    }
}


void getInfo ()
{
    char str[5];

    printf("情報を数字で入力してください( 当てはまらない時はEnter キー)\n");

    while (1) {
        printf("[ロン= 1] [ツモ= 2] : ");
        fgets(str, 5, stdin);
        agari = atoi(str);
        if (1<=agari && agari<=2)
            break;
    }

    while (1) {
        printf("自風は? [東= 1] [南= 2] [西= 3] [北= 4] : ");
        fgets(str, 5, stdin);
        jifu = atoi(str);
        if (1 <= jifu && jifu <= 4)
            break;
    }
    jifu += 30;

    while (1) {
        printf("[リーチ= 1] [一巡目リーチ= 2] [一巡目上がり= 3] : ");
        fgets(str, 5, stdin);
        richi = atoi(str);
        if (richi!=0 && !menzen) {
            printf("門前でないのでは?\n");
            continue;
        }
        if (0<=richi && richi<=3)
            break;
    }

    if (richi==1 || richi==2) {
        while (1) {
            printf("[一発= 1] : ");
            fgets(str, 5, stdin);
            ippatsu = atoi(str);
            if (0<=ippatsu && ippatsu<=1)
                break;
        }
    }

    while (1) {
        printf("[リンシャン= 1] [ハイテイ= 2] [チャンカン= 3] : ");
        fgets(str, 5, stdin);
        rhc = atoi(str);
        if (0<=rhc && rhc<=3)
            break;
    }

    while (1) {
        printf("ドラの数は? : ");
        fgets(str, 5, stdin);
        dora = atoi(str);
        if (0<=dora && dora<=20)
            break;
        else
            printf("Please [0 ~ 20]\n");
    }

    while (1) {
        printf("上がり牌は? [萬子(11~19)] [筒子(21~29)] [索子(31~39)] [字牌(41~47)] : ");
        fgets(str, 5, stdin);
        machi = atoi(str) - 10;
        if (1<=machi && machi<=37 && hai[machi]!=0)
            break;
        printf("本当? \n");
    }

    printf("\n");
}


void determin ()
{
    getInfo();

    toitsu();
    kokushi();

    detHai(1);
}


void detHai (int num)
{
    if (atama > 1)
        return;
    if (num > 37) {
        calc();
        return;
    }

    switch (hai[num]) {
    case 0:
        detHai(num+1);
        break;
    case 1:
        if (num<30 && hai[num+1] >= 1 && hai[num+2] >= 1) {
            shuntsu(num);
            detHai(num+1);
            shuntsu(-num);
        }
        break;
    case 2:
        if (num<30 && hai[num+1] >= 2 && hai[num+2] >= 2) {
            shuntsu(num); shuntsu(num);
            detHai(num+1);
            shuntsu(-num); shuntsu(-num);
        }
        janto(num);
        detHai(num+1);
        janto(-num);
        break;
    case 3:
        if (num<30 && hai[num+1] >= 1 && hai[num+2] >= 1) {
            if (hai[num+1] >= 3 && hai[num+2] >= 3) {
                shuntsu(num); shuntsu(num); shuntsu(num);
                detHai(num+1);
                shuntsu(-num); shuntsu(-num); shuntsu(-num);
            }
            janto(num); shuntsu(num);
            detHai(num+1);
            janto(-num); shuntsu(-num);
        }
        kotsu(num);
        detHai(num+1);
        kotsu(-num);
        break;
    case 4:
        if (num<30 && hai[num+1] >= 2 && hai[num+2] >= 2) {
            if (hai[num+1] == 4 && hai[num+2] == 4) {
                shuntsu(num); shuntsu(num); shuntsu(num); shuntsu(num);
                detHai(num+1);
                shuntsu(-num); shuntsu(-num); shuntsu(-num); shuntsu(-num);
            }
            janto(num); shuntsu(num); shuntsu(num);
            detHai(num+1);
            janto(-num); shuntsu(-num); shuntsu(-num);
        }
        kotsu(num); shuntsu(num);
        detHai(num+1);
        kotsu(-num); shuntsu(-num);
        break;
    default:
        fprintf(stderr, "really?%d\n", num);
        exit(0);
    }
}


void calc ()
{
    int han, fu;
    //disp(men1, men2);
    resetYaku();
    han = sumHan();
    fu = sumFu();
    if (maxhan < han || (maxhan == han && maxfu < fu)) {
        maxhan = han;
        maxfu = fu;
        copyMen();
        copyYaku();
    }
}


void result ()
{
    int point, p, q;
    printf("\n>>>>>>>>>>>>> RESULT <<<<<<<<<<<<<\n");

    if (maxyaku[30]!=1)
        disp(maxmen1, maxmen2);
    else
        dispKokushi();

    printf("-----------------------------------\n");
    point = tensu(&maxhan, &maxfu);
    if (jifu==31) {
        if (point<12000)
            printf(" %d 翻%d 符  ", maxhan, maxfu);
        else if (point==12000)
            printf("満貫  ");
        else if (point==18000)
            printf("跳満  ");
        else if (point==24000)
            printf("倍満  ");
        else if (point==36000)
            printf("三倍満  ");
        else
            printf("役満  ");
    } else {
        if (point<8000)
            printf(" %d 翻%d 符  ", maxhan, maxfu);
        else if (point==8000)
            printf("満貫  ");
        else if (point==12000)
            printf("跳満  ");
        else if (point==16000)
            printf("倍満  ");
        else if (point==24000)
            printf("三倍満  ");
        else
            printf("役満  ");
    }

    if (agari==1) {
        printf("%d 点!\n", point);
    } else {
        if (jifu==31) {
            point /= 3;
            p = point%100;
            if (p!=0)
                point = point -p +100;
            printf("ALL %d 点!\n", point);
        } else {
            point /= 2;
            p = point%100;
            if (p!=0)
                point = point -p +100;
            p = point;

            point /= 2;
            q = point%100;
            if (q!=0)
                point = point -q +100;
            q = point;
            printf("親%d 点! /  子%d 点!\n", p, q);
        }
    }

    printYaku(maxyaku);
    printf("-----------------------------------\n");
}


void disp (int *h1, int *h2)
{
    int i, j, k;
    for (i=0; i<3; i++) {
        if (i==0)
            printf("萬子: ");
        else if (i==1)
            printf("筒子: ");
        else
            printf("索子: ");
        for (j=1; j<10; j++) {
            if (h2[10*i+j]==1)
                printf("%d%d%d  ", j, j, j);
            if (h2[10*i+j]==2)
                printf("%d%d   ", j, j);
            for (k=0; k<(h1[10*i+j]%5); k++)
                printf("%d%d%d  ", j, j+1, j+2);
            if (!menzen) {
                if (h2[10*i+j]==3)
                    printf("(%d%d%d)  ", j, j, j);
                if (h2[10*i+j]>=4)
                    printf("(%d%d%d%d) ", j, j, j, j);
                for(k=0; k<(h1[10*i+j]/5); k++)
                    printf("(%d%d%d)  ", j, j+1, j+2);
            }
        }
        printf("\n");
    }
    printf("字牌: ");
    for (i=31; i<38; i++) {
        j = i%10;
        if (h2[i]==1)
            printf("%d%d%d  ", j, j, j);
        if (h2[i]==2)
            printf("%d%d   ", j, j);
        if (!menzen) {
            if (h2[i]==3)
                printf("(%d%d%d)  ", j, j, j);
            if (h2[i]>=4)
                printf("(%d%d%d%d) ", j, j, j, j);
        }
    }
    printf("\n\n");
}


void dispKokushi ()
{
    int i, j;
    for (i=0; i<3; i++) {
        if (i==0)
            printf("萬子: ");
        else if (i==1)
            printf("筒子: ");
        else
            printf("索子: ");
        for (j=0; j<hai[10*i+1]; j++)
            printf("1 ");
        for (j=0; j<hai[10*i+9]; j++)
            printf("9 ");
        printf("\n");
    }
    printf("字牌: ");
    for (i=31; i<38; i++) {
        for (j=0; j<hai[i]; j++)
            printf("%d ", i%10);
    }
    printf("\n\n");
}


int tensu (int *han, int *fu)
{
    int p, i;
    if (*han >= 5) {
        if (*han < 6)
            return jifu==31? 12000 : 8000;
         else if (*han < 8)
            return jifu==31? 18000 : 12000;
         else if (*han < 11)
            return jifu==31? 24000 : 16000;
         else if (*han < 13)
            return jifu==31? 36000 : 24000;
         else
            return jifu==31? 48000 : 32000;
    }
    if (jifu==31)
        p = *fu * 6;
    else
        p = *fu * 4;
    for (i=0; i<*han+2; i++) {
        p *= 2;
    }

    i = p%100;
    if (i!=0)
        p = p -i +100;

    if (jifu==31 && p>=12000)
        return 12000;
    if (jifu!=31 && p>=8000)
        return 8000;

    return p;
}


void printYaku (int *ya)
{
    int i;
    for (i=35; i>0; i--) {
        if (ya[i] > 0) {
            switch (i) {
            case 1: printf("平和, "); break;
            case 2: printf("断ヤオ九, "); break;
            case 3: printf("一盃口, "); break;
            case 4: printf("立直, "); break;
            case 5: printf("門前清自摸和, "); break;
            case 6: printf("飜牌%d, ", ya[i]); break;
            case 7: if (rhc==1)
                        printf("嶺上開花, ");
                    else if (rhc==2 && agari==1)
                        printf("海底撈月, ");
                    else if (rhc==2 && agari==2)
                        printf("河底撈魚, ");
                    else
                        printf("槍槓, ");
                    break;
            case 8: printf("一発, "); break;
            case 9: printf("ドラ%d, ", ya[i]); break;
            case 10: printf("三色同順, "); break;
            case 11: printf("一気通貫, "); break;
            case 12: printf("対々和, "); break;
            case 13: printf("七対子, "); break;
            case 14: printf("全帯ヤオ九, "); break;
            case 15: printf("混老頭, "); break;
            case 16: printf("三暗刻, "); break;
            case 17: printf("三色同刻, "); break;
            case 18: printf("三槓子, "); break;
            case 19: printf("小三元, "); break;
            case 20: printf("ダブル立直, "); break;
            case 21: printf("純全帯ヤオ九, "); break;
            case 22: printf("混一色, "); break;
            case 23: printf("二盃口, "); break;
            case 24: printf("清一色, "); break;
            case 25: printf("九蓮宝燈"); break;
            case 26: printf("緑一色"); break;
            case 27: printf("四暗刻"); break;
            case 28: printf("四槓子"); break;
            case 29: printf("清老頭"); break;
            case 30: printf("国士無双 "); break;
            case 31: printf("大三元"); break;
            case 32: printf("小四喜"); break;
            case 33: printf("大四喜"); break;
            case 34: printf("字一色"); break;
            case 35: if (agari==1)
                        printf("地和");
                     else
                         printf("天和");
                     break;
            default: break;
            }
            if (i>=25)
                break;
        }
    }
    printf("\n");
}


void resetYaku()
{
    int i;
    for (i=0; i<36; i++) {
        yaku[i] = 0;
    }
}


void copyMen ()
{
    int i;
    for (i=1; i<38; i++) {
        maxmen1[i] = men1[i];
        maxmen2[i] = men2[i];
    }
}


void copyYaku ()
{
    int i;
    for (i=1; i<36; i++) {
        maxyaku[i] = yaku[i];
    }
}


//////////////////////////////////////////////////////////////////////////////////

// 13.toitsu
void toitsu ()
{
    int i;
    if (menzen) {
        for (i=1; i<=37; i++) {
            if (hai[i]!=0 && hai[i]!=2)
                return;
            else
                continue;
        }
        maxyaku[13] = 1;
        maxhan = 2;
        if (dora != 0) {
            maxyaku[9] = dora;
            maxhan += dora;
        }
        if (ippatsu == 1) {
            maxyaku[8] = 1;
            maxhan += 1;
        }
        if (agari==2) {
            maxyaku[5] = 1;
            maxhan += 1;
        }
        maxfu = 25;
        for (i=1; i<38; i++) {
            if (hai[i]==2)
                maxmen2[i] = 2;
        }
    }
}

// 30.kokushi
void kokushi ()
{
    int i;
    if (menzen) {
        for (i=0; i<3; i++) {
            if (hai[10*i+1] == 0 || hai[10*i+9] == 0)
                return ;
        }
        for (i=31; i<=37; i++) {
            if (hai[i] == 0)
                return ;
        }
        maxyaku[30] = 1;
        maxhan = 13;
        maxfu = 0;
    }
}

int sumHan ()
{
    int han=0, i, j, k;

    // 1.pinfu
    if (menzen) {
        if (isShuntsu(machi) || (3<machi%10 && machi%10<9 && isShuntsu(machi-2))) {
            for (i=1; i<30; i++) {
                if (isKotsu(i))
                    break;
            }
            if (i==30) {
                for (i=31; i<38; i++) {
                    if (hai[i] != 0) {
                        if (i==bafu || i==jifu || i>34)
                            break;
                        else if (!isJanto(i))
                            break;
                    }
                }
                if (i==38) {
                    yaku[1] = 1;
                    han += 1;
                }
            }
        }
    }

    // 2.tanyao
    for (i=0; i<3; i++) {
        if (!isNothing(10*i+1) || !isNothing(10*i+9))
            break;
    }
    if (i==3) {
        for (i=31; i<38; i++) {
            if (!isNothing(i))
                break;
        }
        if (i==38) {
            yaku[2] = 1;
            han += 1;
        }
    }

    // 3.iipeko  23.ryanpeko
    k=0;
    if (menzen) {
        for (i=1; i<30; i++) {
            if (isShuntsu(i)) {
                if (shuntsuNum(i) == 4)
                    k += 2;
                else if (shuntsuNum(i) >= 2)
                    k += 1;
            }
        }
        if (k!=0) {
            if (k==1) {
                yaku[3] = 1;
                han += 1;
            }
            if (k==2) {
                yaku[23] = 1;
                han += 3;
            }
        }
    }

    // 4.ri-chi
    if (richi==1) {
        yaku[4] = 1;
        han += 1;
    }

    // 5.menzentsumo
    if (menzen && agari == 2) {
        yaku[5] = 1;
        han += 1;
    }

    // 6.fanpai
    k=0;
    for (i=31; i<38; i++) {
        if (isKotsu(i) || isPon(i) || isMinkan(i) || isAnkan(i)) {
            if (i==bafu)
                k += 1;
            if (i==jifu)
                k += 1;
            if (i>34)
                k += 1;
        }
    }
    if (k!=0) {
        yaku[6] = k;
        han += k;
    }

    // 7.rhc
    if (rhc!=0) {
        yaku[7] = 1;
        han += 1;
    }

    // 8.ippatsu
    if (ippatsu==1) {
        yaku[8] = 1;
        han += 1;
    }

    // 9.dora
    if (dora!=0) {
        yaku[9] = dora;
        han += dora;
    }

    // 10.dojun  17.dokoku
    for (i=1; i<10; i++) {
        for (j=0; j<3; j++) {
            if (isShuntsu(10*j+i))
                continue;
            if (!menzen && isChi(10*j+i))
                continue;
            break;
        }
        if (j==3) {
            yaku[10] = 1;
            if (menzen)
                han += 2;
            else
                han += 1;
        }

        for (j=0; j<3; j++) {
            if (isKotsu(10*j+i))
                continue;
            if (!menzen)
                if (isPon(10*j+i) || isMinkan(10*j+i) || isAnkan(10*j+i))
                    continue;
            break;
        }
        if (j==3) {
            yaku[17] = 1;
            han += 2;
        }
    }

    // 11.ittsu
    for (j=0; j<3; j++) {
        for (i=1; i<10; i+=3) {
            if (isShuntsu(10*j+i))
                continue;
            if (!menzen && isChi(10*j+i))
                continue;
            break;
        }
        if (i==10) {
            yaku[11] = 1;
            if (menzen)
                han += 2;
            else
                han += 1;
        }
    }

    // 12.toitoi
    for (i=1; i<30; i++) {
        if (isShuntsu(i) || isChi(i))
            break;
    }
    if (i==30) {
        yaku[12] = 1;
        han += 2;
    }

    // 14.chanta  21.junchan  15.honro  29.chinro
    k=0;
    for (i=1; i<30; i++) {
        if (isShuntsu(i)) {
            if (i%10==1 || i%10==7)
                k++;
            else
                break;
        }
        if (isKotsu(i) || isJanto(i))
            if (i%10!=1 && i%10!=9)
                break;
        if (!menzen) {
            if (isChi(i)) {
                if (i%10==1 || i%10==7)
                    k++;
                else
                    break;
            }
            if (isPon(i) || isMinkan(i) || isAnkan(i)) {
                if (i%10!=1 && i%10!=9)
                    break;
            }
        }
    }
    if (i==30) {
        for (i=31; i<38; i++) {
            if (isNothing(i))
                continue;
            k += 10;
        }
        if (k%10 != 0) {
            if (k>=10) {
                yaku[14] = 1;
                if (menzen)
                    han += 2;
                else
                    han += 1;
            } else {
                yaku[21] = 1;
                if (menzen)
                    han += 3;
                else
                    han += 2;
            }
        } else {
            if (k>=10) {
                yaku[15] = 1;
                han += 2;
            } else {
                yaku[29] = 1;
                han += 13;
            }
        }
    }

    // 16.sanan  27.su-an
    k=0;
    for (i=1; i<38; i++) {
        if (isKotsu(i)) {
            if (i==machi && agari==1)
                continue;
            else
                k++;
        } else if (isAnkan(i))
            k++;
    }
    if (k==3) {
        yaku[16] = 1;
        han += 2;
    } else if (k==4) {
        yaku[27] = 1;
        han += 13;
    }

    // 18.sankantsu  28.su-kantsu
    k=0;
    for (i=1; i<38; i++) {
        if (!isNothing(i))
            if (isMinkan(i) || isAnkan(i))
                k++;
    }
    if (k==3) {
        yaku[18] = 1;
        han += 2;
    } else if (k==4) {
        yaku[28] = 1;
        han += 13;
    }

    // 19.shosangen  31.daisangen
    for (i=35; i<38; i++) {
        if (isNothing(i))
            break;
        if (!isJanto(i))
            k++;
    }
    if (i==38) {
        if (k==2) {
            yaku[19] = 1;
            han += 2;
        } else {
            yaku[31] = 1;
            han += 13;
        }
    }

    // 20.daburi-
    if (richi==2) {
        yaku[20] = 1;
        han += 2;
    }

    // 21.honitsu  24.chinitsu  34.tsu-i-so-
    k=0;
    for (i=0; i<3; i++) {
        for (j=1; j<10; j++) {
            if (!isNothing(10*i+j))
                break;
        }
        if (j==10)
            k++;
    }
    if (k==2) {
        k=0;
        for (i=31; i<38; i++) {
            if (!isNothing(i))
                k++;
        }
        if (k==0) {
            yaku[24] = 1;
            if (!menzen)
                han += 6;
            else
                han += 5;
        } else {
            yaku[22] = 1;
            if (!menzen)
                han += 3;
            else
                han += 2;
        }
    } else if (k==3) {
        yaku[34] = 1;
        han += 13;
    }

    // 25.churen
    if (!menzen) {
        for (i=0; i<3; i++) {
            if (hai[10*i+1]>=3) {
                for (j=2; j<9; j++) {
                    if (hai[10*i+j]==0)
                        break;
                }
                if (j==9 && hai[10*i+j]>=3) {
                    yaku[25] = 1;
                    han += 13;
                }
            }
        }
    }

    // 26.ryu-i-so-
    for (i=1; i<20; i++) {
        if (!isNothing(i))
            break;
    }
    if (i==20) {
        if (isNothing(21) && isNothing(25) && isNothing(27) && isNothing(29)) {
            for (i=31; i<38; i++) {
                if (i==36) 
                    continue;
                if (!isNothing(i))
                    break;
            }
            if (i==38) {
                yaku[26] = 1;
                han += 13;
            }
        }
    }

    // 32.sho-su-shi-  33.daisu-shi-
    k=0;
    for (i=31; i<35; i++) {
        if (!isNothing(i) && !isJanto(i))
            k++;
    }
    if (k==3) {
        yaku[32] = 1;
        han += 13;
    } else if (k==4) {
        yaku[33] = 1;
        han += 13;
    }

    // 35.(tenho-, chi-ho-)
    if (richi==3) {
        yaku[35] = 1;
        han += 13;
    }

    return han;
}


int sumFu ()
{
    int fu=20, i, k;

    if (yaku[1]==1) {
        if (agari==2)
            return 20;
        else
            return 30;
    }

    // agari
    if (agari==1 && menzen)
        fu += 10;
    if (agari==2)
        fu += 2;

    //mentsu,  janto
    for (i=1; i<38; i++) {
        k=0;
        if (!isNothing(i)) {
            if (isPon(i))
                k = 2;
            else if (isKotsu(i))
                k = 4;
            else if (isMinkan(i))
                k = 8;
            else if (isAnkan(i))
                k = 16;
            if (i%10==1 || i%10==9 || i>30)
                k *= 2;
            if (isJanto(i)) {
                if (i==bafu)
                    fu += 2;
                if (i==jifu)
                    fu += 2;
                if (i>34)
                    fu += 2;
            }
        }
        fu += k;
    }

    //machi
    if (isJanto(machi))
        k = 2;
    else if (isShuntsu(machi-1))
        k = 2;
    else if (machi%10==3 && isShuntsu(machi-2))
        k = 2;
    else if (machi%10==7 && isShuntsu(machi))
        k = 2;
    else
        k = 0;
    fu += k;

    k = fu%10;
    if (k!=0)
        fu = fu -k +10;

    return fu;
}


void shuntsu (int n)
{
    if (n>0) {
        hai[n]--; hai[n+1]--; hai[n+2]--;
        men1[n] += 1;
    } else {
        hai[-n]++; hai[-n+1]++; hai[-n+2]++;
        men1[-n] -= 1;
    }
}

void kotsu (int n)
{
    if (n>0) {
        hai[n] -= 3;
        men2[n] = 1;
    } else {
        hai[-n] += 3;
        men2[-n] = 0;
    }
}

void janto (int n)
{
    if (n>0) {
        hai[n] -= 2;
        men2[n] = 2;
        atama++;
    } else {
        hai[-n] += 2;
        men2[-n] = 0;
        atama--;
    }
}

void chi (int n)
{
    men1[n] += 5;
    naki[n]++; naki[n+1]++; naki[n+2]++;
}

void pon (int n)
{
    men2[n] = 3;
    naki[n] += 3;
}

void ankan (int n)
{
    men2[n] = 5;
    naki[n] += 4;
}

void minkan (int n)
{
    men2[n] = 4;
    naki[n] += 4;
}


int isShuntsu (int n)
{
    if (men1[n]%5 != 0)
        return 1;
    else
        return 0;
}

int isKotsu (int n)
{
    if (men2[n] == 1)
        return 1;
    else
        return 0;
}

int isJanto (int n)
{
    if (men2[n] == 2)
        return 1;
    else
        return 0;
}

int isChi (int n)
{
    if (men1[n] > 4)
        return 1;
    else
        return 0;
}

int isPon (int n)
{
    if (men2[n] == 3)
        return 1;
    else
        return 0;
}

int isMinkan (int n)
{
    if (men2[n] == 4)
        return 1;
    else
        return 0;
}

int isAnkan (int n)
{
    if (men2[n] == 5)
        return 1;
    else
        return 0;
}

int isNothing (int n)
{
    if (men1[n] == 0 && men2[n] == 0)
        return 1;
    else
        return 0;
}

int shuntsuNum (int n)
{
    return (men1[n]%5);
}

int chiNum (int n)
{
    return (men1[n]/5);
}
