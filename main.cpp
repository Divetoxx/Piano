#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <string>
#include <cmath>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

const std::vector<BYTE> MELODY1 = {79,80,82,82,82,82,82,84,82,79,80,82,82,82,82,82,84,82,87,86,87,86,84,77,77,80,82,84,84,84,84,84,86,84,80,82,84,84,84,84,84,86,84,89,87,86,84,81,82,79,79,80,82,82,82,82,82,84,82,79,80,82,82,82,82,82,84,82,91,89,87,86,83,84,84,92,89,84,89,84,89,84,89,91,87,83,84,83,84,83,84,87,84,80,79,77,82,79,82,87,87,87,87,87,87,87,77,82,87,87,87,87,87,87,87,89,91,92,84,86,91,87};
const std::vector<BYTE> MELODY2 = {76,75,76,77,76,72,72,71,69,72,76,76,81,76,79,78,77,74,83,81,80,77,76,74,76,74,72,69,71,72,71,69,71,74,77,76,74,72,71,72,69,72,76,81,84,81,83,84,83,81,83,86,89,88,86,84,83,81,80,81,83,80,83,81,68,71,76,74,72,77,76,74,72,71,69,68,64,68,71,76,74,72,69,77,76,74,72,71,69,68,52,59,57,56,59,62,60,59,62,65,64,62,68,71,69,68,71,74,72,71,74,77,76,76,76,76,76,76,76,76,76,76,75,76,77,76,72,72,71,69,72,76,76,81,76,79,78,77,74,83,81,80,77,76,74,76,74,72,69,71,72,71,69,71,74,77,76,74,72,71,72,69,72,76,81,84,81,83,84,83,81,83,86,89,88,86,84,83,81,80,81,83,80,83,81,69,64,66,68,69,71,72,69,71,64,68,69,71,72,74,71,72,67,69,71,72,74,76,72,74,67,71,72,74,76,77,74,76,75,76,77,76,74,72,72,71,69,72,71,71,76,64,68,69,68,69,71,72,71,72,74,76,72,71,71,76,64,68,69,68,72,71,71,69};
const std::vector<BYTE> MELODY3 = {52,60,59,57,56,57,52,64,62,60,59,60,57,65,64,62,60,59,60,62,65,64,62,60,62,59,52,60,59,57,56,57,52,64,62,60,59,60,57,65,64,62,60,59,60,62,65,64,62,60,62,59,64,64,55,55,55,55,53,52,53,53,62,62,53,53,53,53,52,50,52,52,47,50,53,57,62,60,59,57,57,56,47,50,53,57,62,60,59,57,57,56,57};
const std::vector<BYTE> MELODY4 = {63,64,62,60,59,57,68,69,67,65,65,61,62,60,59,57,55,66,67,65,64,64,63,64,62,60,59,57,68,69,67,65,65,64,62,64,69,57,59,64,52,57};
const std::vector<BYTE> MELODY5 = {72,69,64,64,69,72,71,69,71,68,64,69,70,69,67,64,62,61,61,62,64,67,65,64,65,77,74,70,70,74,77,76,74,76,72,69,72,69,75,72,72,71,69,71,76,77,76,77,76,77,76,77,52,57,59,60,59,57,52,52,57,59,60,64,67,65,59,59,60,62,60,62,64,62,64,65,64,62,60,59,59,60,55,60,62,60,59,57,56};
const std::vector<BYTE> MELODY6 = {74,67,69,71,72,74,67,67,76,72,74,76,78,79,67,67,72,74,72,71,69,71,72,71,69,67,66,67,69,71,67,71,69,74,67,69,71,72,74,67,67,76,72,74,76,78,79,67,67,72,74,72,71,69,71,72,71,69,67,69,71,69,67,66,67};
const std::vector<BYTE> MELODY7 = {74,72,70,69,67,67,66,75,74,72,70,69,69,67,79,77,79,75,77,74,75,77,75,77,74,75,72,74,75,74,75,72,74,70,72,74,79,81,82,81,79,78,79,77,62,63,65,63,62,60,72,75,79,74,70,74,79,72,68,70,72,70,68,67,67,69,70,69,67,66,66,67,69,67,66,67};
const std::vector<BYTE> MELODY8 = {62,63,64,72,64,72,64,72,72,74,75,76,72,74,76,71,74,72,62,63,64,72,64,72,64,72,69,67,66,69,72,76,74,72,69,74,62,63,64,72,64,72,64,72,72,74,75,76,72,74,76,71,74,72,72,74,76,72,74,76,72,74,72,76,72,74,76,72,74,72,76,72,74,76,71,74,72};
const std::vector<BYTE> MELODY9 = {60,69,69,67,69,65,60,60,60,69,69,70,67,72,72,62,62,70,70,69,67,65,60,69,69,67,69,65,72,62,62,70,70,69,67,65,60,69,69,67,69,65};
const std::vector<BYTE> MELODY10 = {81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,81,78,76,75,75,75,81,78,76,75,75,75,81,78,80,76,73,81,80,78,76,78,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,81,78,76,75,75,75,81,78,76,75,75,75,81,78,80,76,73,81,80,78,76,78,76,84,76,83,76,76,76,81,76,80,76,76,76,84,76,83,76,76,76,81,76,80,76,76,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,74,71,69,68,68,68,74,71,69,68,68,68,74,71,73,69,66,74,73,71,69,71,69,84,76,83,76,76,76,81,76,80,76,76,76,84,76,83,76,76,76,81,76,80,76,76,76,81,80,81,83,81,69,73,76,76,74,74,74,73,74,76,74,64,71,74,74,73,73,74,71,69,68,68,68,74,71,69,68,68,68,74,71,73,69,66,74,73,71,69,71,69};
const std::vector<BYTE> MELODY11 = {74,74,71,71,74,74,69,69,71,72,74,76,78,74,74,74,71,71,74,74,69,69,81,80,81,83,76,81,74,83,83,81,79,79,78,78,79,81,78,76,74,79,79,79,76,76,79,79,74,74,74,76,79,74,81,79};
const std::vector<BYTE> MELODY12 = {83,83,83,83,83,83,83,86,79,81,83,84,84,84,84,84,83,83,83,83,81,81,83,81,86,83,83,83,83,83,83,83,86,79,81,83,84,84,84,84,84,83,83,83,86,86,84,81,79};
const std::vector<BYTE> MELODY13 = {65,77,75,75,73,73,72,72,70,70,72,65,65,77,75,75,73,73,72,72,70,70,72,72,65,70,82,80,80,78,78,77,77,68,80,78,78,77,77,75,73,75,72,69,70,77,89,87,87,85,85,84,84,82,82,84,77,77,89,87,87,85,85,84,84,82,82,84,84,77,82,94,92,92,90,90,89,89,80,92,90,90,89,89,87,85,87,84,81,82,49,53,58,60,61,61,49,53,58,60,61,61,54,54,58,63,65,66,66,54,58,63,65,66,66,68,65,77,75,75,73,73,72,72,70,70,72,65,65,77,75,75,73,73,72,72,70,70,72,72,65,70,82,80,80,78,78,77,77,68,80,78,78,77,77,75,73,75,72,69,70,77,89,87,87,85,85,84,84,82,82,84,77,77,89,87,87,85,85,84,84,82,82,84,84,77,82,94,92,92,90,90,89,89,80,92,90,90,89,89,87,85,87,84,81,82};
const std::vector<BYTE> MELODY14 = {71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,84,83,81,79,81,83,81,79,81,83,81,79,78,76,71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,84,83,81,79,81,83,81,79,81,83,81,79,78,76,76,77,79,79,81,79,77,76,74,67,76,77,79,79,81,79,77,76,74,72,74,76,76,77,76,74,72,71,64,72,74,76,76,77,76,74,72,71,71,69,68,69,72,74,72,71,72,76,77,76,75,76,83,81,80,81,83,81,80,81,84,81,83,84,83,81,80,81,76,77,74,72,71,69,69,71,73,69,71,73,71,69,68,66,68,69,71,68,64,69,71,73,69,71,73,71,69,68,66,71,68,64,69,85,86,85,83,81,83,81,80,78,81,80,78,77,78,80,77,73,75,77,73,78,77,78,80,81,80,81,83,85,84,85,84,85,86,85,83,81,83,81,80,78,81,80,78,76,78,80,76,73,75,76,73,75,76,78,75,72,73,75,72,73,85,86,85,83,81,83,81,80,78,81,80,78,77,78,80,77,73,75,77,73,78,77,78,80,81,80,81,83,85,84,85,84,85,86,85,83,81,83,81,80,78,81,80,78,76,78,80,76,73,75,76,73,75,76,78,75,72,73,75,72,73,76,74,73,71,69,71,73,74,76,78,80,81,81,80,78,76,76,74,73,71,69,71,73,74,76,78,80,81,82,83,76,74,73,71,69,71,73,74,76,78,80,81,81,80,78,76,76,74,73,71,73,76,69,73,71,74,68,71,69};
const std::vector<BYTE> MELODY15 = {62,62,66,63,62,66,66,69,67,66,67,67,70,69,67,66,63,66,63,66,62,62,66,63,62,66,66,69,67,66,67,67,70,69,67,66,63,66,63,62,66,66,63,62,62,62,63,63,62,60,60,60,60,63,62,60,60,67,66,63,66,63,62,66,66,63,62,62,62,63,63,62,60,60,60,60,63,62,60,60,67,66,63,66,63,62,67,67,67,67,67,67,67,67,70,69,67,70,69,67,67,67,70,69,67,70,69,67,69,69,72,70,69,72,70,69,69,69,72,70,69,72,70,69,69,69,74,69,69,74,62,62,74,72,70,69,67};
const std::vector<BYTE> MELODY16 = {72,64,64,71,69,71,63,63,71,62,62,69,68,69,61,61,69,60,60,69,68,69,59,59,59,62,64,65,65,64,62,62,60,60,64,62,60,59,57,60,59,72,64,64,71,69,71,63,63,71,62,62,69,68,69,61,61,69,60,60,69,68,69,59,59,59,62,64,65,64,62,64,65,69,68,69,71,64,72};
const std::vector<BYTE> MELODY17 = {69,74,76,77,79,81,81,82,81,82,86,81,79,79,76,77,77,76,74,76,77,74,69,74,76,77,79,81,81,82,81,82,86,81,79,79,76,77,77,76,74,76,77,74,74,86,86,86,84,86,84,82,81,74,86,86,86,84,86,84,82,81,79,79,76,77,77,79,81,82,84,81,79,77,79,79,77,77,77,76,74,76,77,74,79,79,76,77,77,79,81,82,84,81,79,77,79,79,77,77,77,76,74,76,77,74};
const std::vector<BYTE> MELODY18 = {69,70,69,67,69,72,70,69,67,67,65,69,67,65,65,67,69,65,67,69,70,69,67,69,74,72,74,72,72,70,69,70,70,72,70,69,70,74,72,70,69,69,72,70,66,67,67,69,70,67,74,72,74,72,70,70,69,68,68,69,69,74,76,74,73,76,70,68,69,77,76,74,76,74,73,76,70,72,72,74,72,74,72,71,72,71,72,74,76,77,79,81,81,81,81,82,81,79,77,79,77,76,74,76,74,72,70,72,70,69,67,74,72,71,72,76,77,72,69,69,67,66,67,71,72,70,67,65};


struct LanguageInterface {
    const wchar_t* selectLbl;
    const wchar_t* hintLbl;
    const wchar_t* playBtn;
    const wchar_t* resetBtn;
};

const LanguageInterface LANGS[14] = {
    { L"Выберите мелодию:", L"ЛЕВАЯ мышь или ПРОБЕЛ на клавиатуре", L"ИГРАТЬ НОТУ", L"Сброс" },
    { L"Melodies:", L"LEFT mouse or SPACE on keyboard", L"PLAY NOTE", L"Reset" },
    { L"Seleccionar melodía:", L"Ratón IZQUIERDO o ESPACIO en el teclado", L"TOCAR NOTA", L"Reiniciar" },
    { L"Seleziona melodia:", L"Mouse SINISTRO o SPAZIO sulla tastiera", L"SUONA NOTA", L"Reimposta" },
    { L"Melodie auswählen:", L"LINKE Maustaste oder LEERTASTE auf der Tastatur", L"NOTE SPIELEN", L"Zurücksetzen" },
    { L"Choisir une mélodie:", L"Souris GAUCHE ou ESPACE au clavier", L"JOUER LA NOTE", L"Réinitialiser" },
    { L"メロディを選択:", L"左マウスまたはキーボードのスペースキー", L"音符を再生", L"リセット" },
    { L"选择旋律:", L"鼠标左键或键盘空格键", L"演奏音符", L"重置" },
    { L"धुन चुनें:", L"बायां माउस या कीボード पर स्पेस", L"नोट बजाएं", L"रीसेट" },
    { L"Selecionar melodia:", L"Mouse ESQUERDO ou ESPAÇO no teclado", L"TOCAR NOTA", L"Resetar" },
    { L"Melodi seçin:", L"SOL fare veya klavyede BOŞLUK", L"NOTAYI ÇAL", L"Sıfırla" },
    { L"Pilih melodi:", L"Mouse KIRI atau SPASI di keyboard", L"MAINKAN NADA", L"Reset" },
    { L"اختر اللحن:", L"الماوس الأيسر أو المسافة на клавиатуре", L"عزف النوتة", L"إعادة تعيين" },
    { L"בחר מנגינה:", L"עכבר שמאלי או רווח במקלדת", L"נגן תו", L"איפוס" }
};

int currentLang = 1;

struct MultiLangMelody {
    const wchar_t* names[14]; // Массив из 9 языковых названий
    const std::vector<BYTE>* melodyData;
};

std::vector<MultiLangMelody> multilang_melodies = {

    {{ 
        L"Вальс-шутка, Дмитрий Шостакович", L"Waltz-Joke, Dmitri Shostakovich", 
        L"Vals-Broma, Dmitri Shostakóvich", L"Valzer-Scherzo, Dmitrij Šostakovič", 
        L"Walzer-Scherz, Dmitri Schostakowitsch", L"Valse-Plaisanterie, Dmitri Chostakovitch", 
        L"ワルツ・ジョーク (ショスタコーヴィチ)", L"诙谐圆舞曲 (肖斯塔科维奇)", L"वाल्ट्ज-जोक (दिमित्री शोस्ताकोविच)",
        L"Valsa-Piada, Dmitri Shostakovich", L"Şaka Valsi, Dmitri Şostakoviç", L"Waltz-Joke, Dmitri Shostakovich",
        L"1. فالس المزحة، ديمتري شوستاكوفيتش", L"1 .ואלס הבדיחה, דמיטרי שוסטקוביץ'"
    }, &MELODY1},


    {{ 
        L"Полонез 13, Михаил Огинский", L"Polonaise 13, Michal Kleofas Oginski", 
        L"Polonesa 13, Michał Kleofas Ogiński", L"Polacca 13, Michał Kleofas Ogiński", 
        L"Polonaise 13, Michał Kleofas Ogiński", L"Polonaise 13, Michał Kleofas Ogiński", 
        L"ポロネーズ第13番 (オギンスキ)", L"第13号波兰舞曲 (奥金斯基)", L"पोलोनेज़ 13 (मिखाल ओगिंस्की)",
        L"Polonésia 13, Michal Oginski", L"Polonez 13, Michal Oginski", L"Polonaise 13, Michal Oginski",
        L"2. بولونيز 13، ميخال أوغينسكي", L"2. פולונז 13, מיכאל אוגינסקי"
    }, &MELODY2},


    {{ 
        L"Город Золотой, Владимир Вавилов", L"The City of Gold, Vladimir Vavilov", 
        L"La Ciudad de Oro, Vladimir Vavilov", L"La Città d'Oro, Vladimir Vavilov", 
        L"Die goldene Stadt, Wladimir Wawilow", L"La Ville d'Or, Vladimir Vavilov", 
        L"黄金の街 (ウラジーミル・ヴァヴィロフ)", L"黄金之城 (弗拉基米尔·瓦维洛夫)", L"द सिटी ऑफ गोल्ड (व्लादिमीर वाविलोव)",
        L"A Cidade de Ouro, Vladimir Vavilov", L"Altın Şehir, Vladimir Vavilov", L"Kota Emas, Vladimir Vavilov",
        L"3. المدينة الذهبية، فلاديمير فافيلوف", L"3. עיר הזהב, ולדימיר ואווילוב"
    }, &MELODY3},


    {{ 
        L"Шербурские зонтики, Мишель Легран", L"The Umbrellas of Cherbourg, Michel Legrand", 
        L"Los Paraguas de Cherburgo, Michel Legrand", L"I Ombrelli di Cherbourg, Michel Legrand", 
        L"Die Regenschirme von Cherbourg, Michel Legrand", L"Les Parapluies de Cherbourg, Michel Legrand", 
        L"シェルブールの雨傘 (ミシェル・ルグラン)", L"瑟堡的雨伞 (米歇尔·乐格兰)", L"द अम्ब्रेलाज़ ऑफ़ चेरबर्ग (मिशेल लेग्रैंड)",
        L"Os Guarda-Chuvas de Cherburgo, Michel Legrand", L"Cherbourg Şemsiyeleri, Michel Legrand", L"Payung Cherbourg, Michel Legrand",
        L"4. مظلات شيربورغ، ميشيل ليغراند", L"4. מטריות שרבורג, מישל לגראן"
    }, &MELODY4},


    {{ 
        L"Бабье лето, Джо Дассен", L"L'Ete indien, Joe Dassin", 
        L"El verano indio, Joe Dassin", L"L'Estate indiana, Joe Dassin", 
        L"Indian Summer, Joe Dassin", L"L'Été indien, Joe Dassin", 
        L"インディアン・サマー (ジョー・ダッサン)", L"印第安夏天 (乔·达辛)", L"ल एते इन्दियन (जो दासिन)",
        L"O Verão Índio, Joe Dassin", L"Pastırma Yazı, Joe Dassin", L"Musim Panas India, Joe Dassin",
        L"5. الصيف الهندي، جو داسين", L"5. הקיץ האינדיאני, ג'ו דאסן"
    }, &MELODY5},


    {{ 
        L"Менуэт, Иоганн Себастьян Бах", L"Minuet in G major, J. S. Bach", 
        L"Minueto en sol mayor, J. S. Bach", L"Minuetto in sol maggiore, J. S. Bach", 
        L"Menuett in G-Dur, J. S. Bach", L"Menuet en sol majeur, J. S. Bach", 
        L"メヌエット ト長調 (J.S.バッハ)", L"G大调小步舞曲 (巴赫)", L"मिनुएट इन जी मेजर (जे. एस. बाख)",
        L"Minueto em Sol Maior, J. S. Bach", L"Sol Majör Minuet, J. S. Bach", L"Minuet dalam G mayor, J. S. Bach",
        L"6. مِنْوِيت في جي كبير، يوهان باخ", L"6. מנואט בסול מז'ור, י. ס. באך"
    }, &MELODY6},


    {{ 
        L"Адажио, Ремо Джадзотто", L"Adagio in G minor, Remo Giazotto", 
        L"Adagio en sol menor, Remo Giazotto", L"Adagio in sol minore, Remo Giazotto", 
        L"Adagio in g-Moll, Remo Giazotto", L"Adagio en sol mineur, Remo Giazotto", 
        L"アダージョ ト短調 (レモ・ジャゾット)", L"g短调柔板 (雷莫·贾佐托)", L"अदाजियो इन जी माइनर (रेमो गियाज़ोटो)",
        L"Adágio em Sol Menor, Remo Giazotto", L"Sol Minör Adagio, Remo Giazotto", L"Adagio dalam G minor, Remo Giazotto",
        L"7. أداجيو في جي صغير، ريمو جيازوتو", L"7. אדאג'ו בסול מינור, רמו ג'אזוטו"
    }, &MELODY7},


    {{ 
        L"Артист эстрады, Скотт Джоплин", L"The Entertainer, Scott Joplin", 
        L"El Animador, Scott Joplin", L"L'In intrattenitore, Scott Joplin", 
        L"The Entertainer, Scott Joplin", L"L'Animateur, Scott Joplin", 
        L"ジ・エンターテイナー (スコット・ジョップリン)", L"演艺人 (斯科特·乔普林)", L"द एंटरटेनर (स्कॉट जोपलिन)",
        L"O Animador, Scott Joplin", L"Eğlendirici, Scott Joplin", L"The Entertainer, Scott Joplin",
        L"8. المطرب، سكوت جوبلين", L"8. הבדרן, סקוט ג'ופלין"
    }, &MELODY8},


    {{ 
        L"В лесу родилась елочка, Л. Бекман", L"A Spruce Was Born in the Forest, L. Beckman", 
        L"Un Abeto Nació en el Bosque, L. Beckman", L"Un Abete è Nato nel Bosco, L. Beckman", 
        L"Ein Tannenbaum wurde im Wald geboren", L"Un Sapin est Né dans la Forêt, L. Beckman", 
        L"森で生まれた樅の木 (ベックマン)", L"森林里长出一棵小松树 (贝克曼)", L"ए स्推ूस वाज़ बॉर्न इन द फॉरेस्ट (बेकमैन)",
        L"Um Abeto Nasceu na Floresta, L. Beckman", L"Ormanda Bir Ladin Doğdu, L. Beckman", L"Sebuah Cemara Lahir di Hutan, L. Beckman",
        L"9. ولدت شجرة تنوب في الغابة، ل. بيكمان", L"9. עץ אשוח נולד ביער, ל. בקמן"
    }, &MELODY9},


    {{ 
        L"Менуэт, Луиджи Боккерини", L"String Quintet in E major, L. Boccherini", 
        L"Quinteto de cuerda en mi mayor, Boccherini", L"Quintetto d'archi in mi maggiore, Boccherini", 
        L"Streichquintett in E-Dur, L. Boccherini", L"Quintette à cordes en mi majeur, Boccherini", 
        L"弦楽五重奏曲ホ長調 (ボッケリーニ)", L"E大调弦乐五重奏 (博凯里尼)", L"स्ट्रिंग क्विंटेट इन ई मेजर (बोकेरिनी)",
        L"Quinteto de Cordas em Mi Maior, Boccherini", L"Mi Majör Yaylı Çalgılar Beşlisi, Boccherini", L"String Quintet dalam E mayor, Boccherini",
        L"10. خماسية وتارية في إي كبير، بوكريني", L"10. חמישיית כלי מיתר במי מז'ור, בוקריני"
    }, &MELODY10},


    {{ 
        L"Америка прекрасна, Сэмюэл Уорд", L"America The Beautiful, Samuel A. Ward", 
        L"América la Bella, Samuel A. Ward", L"America la Bella, Samuel A. Ward", 
        L"America The Beautiful, Samuel A. Ward", L"L'Amérique la Belle, Samuel A. Ward", 
        L"美しきアメリカ (サミュエル・A・ウォード)", L"美丽的大陆 (塞缪尔·A·沃德)", L"अमेरिका द ब्यूटीफुल (सैमुअल ए. वार्ड)",
        L"América, a Linda, Samuel A. Ward", L"Güzel Amerika, Samuel A. Ward", L"Amerika Yang Indah, Samuel A. Ward",
        L"11. أمريكا الجميلة، صموئيل وارد", L"11. אמריקה היפה, סמואל א. וורד"
    }, &MELODY11},


    {{ 
        L"Джингл Белс, Джеймс Лорд Пирпонт", L"Jingle Bells, James Lord Pierpont", 
        L"Cascabeles, James Lord Pierpont", L"Campane di Natale, James Lord Pierpont", 
        L"Jingle Bells, James Lord Pierpont", L"Vive le Vent, James Lord Pierpont", 
        L"ジングルベル (ジェームズ・ロード・ピアポント)", L"铃儿响叮当 (詹姆斯·洛德·皮尔庞特)", L"जिंगल बेल्स (जेम्स लॉर्ड पियरपोंट)",
        L"Jingle Bells, James Lord Pierpont", L"Jingle Bells, James Lord Pierpont", L"Jingle Bells, James Lord Pierpont",
        L"12. جنجل بيلز، جيمس لورد بيربونت", L"12. ג'ינגל בלס, ג'ייмс לורד פיירפונט"
    }, &MELODY12},


    {{ 
        L"Тот самый Мюнхгаузен, А. Рыбников", L"The Very Same Munchhausen, A. Rybnikov", 
        L"El mismísimo Münchhausen, A. Rýbnikov", L"Il vero Barone di Münchhausen, A. Rybnikov", 
        L"Münchhausen-Thema, Alexei Rybnikow", L"Le Même Munchhausen, A. Rybnikov", 
        L"あのミュンヒハウゼン (アレクセイ・リブニコフ)", L"那个男爵慕尼黑曾 (阿列克谢·雷布尼科夫)", L"द वेरी सेम मुनशौसेन (ए. रयब्निकोव)",
        L"O Próprio Munchhausen, A. Rybnikov", L"O Meşhur Munchhausen, A. Rybnikov", L"Munchhausen Yang Sama, A. Rybnikov",
        L"13. ذلك الميونخهاوزن نفسه، أ. ريبنيكوف", L"13. אותו מינכהאוזן, א. ריבניקוב"
    }, &MELODY13},


    {{ 
        L"Турецкий марш, В. А. Моцарт", L"Alla turca (Turkish March), W. A. Mozart", 
        L"Marcha Turca, W. A. Mozart", L"Marcia Turca, W. A. Mozart", 
        L"Türkischer Marsch, W. A. Mozart", L"Marche Turque, W. A. Mozart", 
        L"トルコ行進曲 (W.A.モーツァルト)", L"土耳其进行曲 (莫扎特)", L"अल्ला तुर्का (तुर्की मार्च) (मोज़ार्ट)",
        L"Marcha Turca, W. A. Mozart", L"Türk Marşı, W. A. Mozart", L"Alla Turca (Maret Turki), W. A. Mozart",
        L"14. المارش التركي، و. أ. موزارت", L"14. המצעד הטורקי, ו. א. מוצרט"
    }, &MELODY14},


    {{ 
        L"Хава нагила, еврейская песня", L"Hava Nagila (Jewish Folk Song)", 
        L"Hava Nagila (Canción folclórica judía)", L"Hava Nagila (Canto popolare ebraico)", 
        L"Hava Nagila (Jüdisches Volkslied)", L"Hava Nagila (Chanson populaire juive)", 
        L"ハヴァ・ナギラ (ユダヤ民謡)", L"哈瓦·纳吉拉 (犹太民歌)", L"हवा नगीला (यहूदी लोक गीत)",
        L"Hava Nagila (Canção folclórica judaica)", L"Hava Nagila (Yahudi Halk Şarkısı)", L"Hava Nagila (Lagu Rakyat Yahudi)",
        L"15. هافا ناجيلا (أغنية شعبية يهودية)", L"15. הבה נגילה (שיר עם יהודי)"
    }, &MELODY15},


    {{ 
        L"Где-то далеко, Микаэл Таривердиев", L"Somewhere Far Away, Mikael Tariverdiev", 
        L"En algún lugar lejano, Mikael Tariverdiev", L"Da qualche parte lontano, M. Tariverdiev", 
        L"Irgendwo ganz weit weg, Mikael Tariwerdiew", L"Quelque Part au Loin, Mikael Tariverdiev", 
        L"遠くのどこかで (ミカエル・タリヴェルディエフ)", L"遥远的某处 (米卡埃尔·塔里弗尔迪耶夫)", L"समव्हेयर फार अवे (मिकाएल तारिवरदीव)",
        L"Algum Lugar Distante, Mikael Tariverdiev", L"Uzaklarda Bir Yerlerde, M. Tariverdiev", L"Di Suatu Tempat Yang Jauh, M. Tariverdiev",
        L"16. في مكان ما بعيداً، ميكائيل تاريفيرديف", L"16. אי שם רחוק, מיכאל טאריברדייב"
    }, &MELODY16},


    {{ 
        L"Атиква, Джузеппе Ченчи", L"Hatikvah, Giuseppe Cenci", 
        L"Hatikvah, Giuseppe Cenci", L"Hatikvah, Giuseppe Cenci", 
        L"Hatikvah, Giuseppe Cenci", L"Hatikvah, Giuseppe Cenci", 
        L"ハティクヴァ (ジュゼッペ・チェンチ)", L"希望之歌 (朱塞佩·琴契)", L"हातिकवाह (जूसेपे सेंसी)",
        L"Hatikvah, Giuseppe Cenci", L"Hatikvah, Giuseppe Cenci", L"Hatikvah, Giuseppe Cenci",
        L"17. هاتيكفاه، جوزيبي سينشي", L"17. התקווה, ג'וזפה סנצ'י"
    }, &MELODY17},


    {{ 
        L"Каста Дива, Винченцо Беллини", L"Norma: Casta diva, Vincenzo Bellini", 
        L"Casta Diva (Norma), Vincenzo Bellini", L"Casta Diva (Norma), Vincenzo Bellini", 
        L"Casta Diva (Norma), Vincenzo Bellini", L"Casta Diva (Norma), Vincenzo Bellini", 
        L"カスタ・ディーヴァ (ヴィンチェンツォ・ベッリーニ)", L"圣洁的女神 (文琴佐·贝里尼)", L"कास्टा दिवा (विन्сеन्ज़ो बेलिनी)",
        L"Casta Diva, Vincenzo Bellini", L"Casta Diva, Vincenzo Bellini", L"Casta Diva, Vincenzo Bellini",
        L"18. كاستا ديفا، فينتشينزو بيليني", L"18. קסטה דיווה, וינצ'נצו בליני"
    }, &MELODY18}
};

HWND hComboBox = nullptr;
HWND hBtnPlay = nullptr;
HWND hBtnReset = nullptr;


const unsigned int DISCR = 48000;
const float AMPLITUDE = 0.35f; 
const float PI = 3.1415926535f;

const float HZS[] = {
    131.0f, 139.0f, 147.0f, 156.0f, 165.0f, 175.0f, 185.0f, 196.0f, 208.0f, 220.0f, 233.0f, 247.0f,
    262.0f, 277.0f, 294.0f, 311.0f, 330.0f, 349.0f, 370.0f, 392.0f, 415.0f, 440.0f, 466.0f, 494.0f,
    523.0f, 554.0f, 587.0f, 622.0f, 659.0f, 698.0f, 740.0f, 784.0f, 831.0f, 880.0f, 932.0f, 988.0f,
    1047.0f, 1109.0f, 1175.0f, 1245.0f, 1319.0f, 1397.0f, 1480.0f, 1568.0f, 1661.0f, 1760.0f, 1865.0f, 1976.0f,
    2093.0f, 2217.0f, 2349.0f, 2489.0f, 2637.0f, 2794.0f, 2960.0f, 3136.0f, 3322.0f, 3520.0f, 3729.0f, 3951.0f,
    4186.0f, 4435.0f, 4699.0f, 4978.0f, 5274.0f, 5588.0f, 5920.0f, 6272.2f, 6644.9f, 7040.0f, 7458.6f, 7902.1f,
    8372.0f, 8869.8f, 9397.3f, 9956.1f, 10548.1f, 11175.3f, 11839.8f, 12543.9f, 13289.8f, 14080.0f, 14917.2f, 15804.3f,
    16744.0f, 17739.7f, 18794.5f, 19912.1f, 21096.2f, 22350.6f, 23679.6f, 25087.7f, 26579.5f, 28160.0f, 29834.5f, 31608.5f,
    33488.1f, 35479.4f, 37589.1f, 39824.3f
};


std::vector<std::vector<float>> precomputedSounds;

size_t selectedIdx = 0;
size_t currentNoteIdx = 0;

const int ID_COMBOBOX = 5001;
const int ID_PLAY_BTN = 5002;
const int ID_RESET_BTN = 5003;

struct ActiveVoice {
    int soundIdx = -1;
    size_t sampleOffset = 0;
};
const int MAX_VOICES = 1;
ActiveVoice activeVoices[MAX_VOICES];

HWAVEOUT hWaveOut = nullptr;
const int BUFFER_SAMPLES = 2048;
short audioBuffer1[BUFFER_SAMPLES * 2];
short audioBuffer2[BUFFER_SAMPLES * 2];
WAVEHDR waveHdr1, waveHdr2;
MMRESULT hTimer = 0;

std::vector<float> GenerateNoteData(BYTE midiNote) {
    std::vector<float> samples;
    int numSamples = 24000;
    samples.reserve(numSamples);
    
    int baseIdx = (int)midiNote - 40;
    if (baseIdx < 0) baseIdx = 0;

    int idx1 = baseIdx;
    int idx2 = baseIdx + 12;
    int idx3 = baseIdx + 28;
    int idx4 = baseIdx + 34;

    float f1 = (idx1 < 100) ? HZS[idx1] : 440.0f * powf(2.0f, (float)(midiNote - 69) / 12.0f);
    float f2 = (idx2 < 100) ? HZS[idx2] : 440.0f * powf(2.0f, (float)(midiNote + 12 - 69) / 12.0f);
    float f3 = (idx3 < 100) ? HZS[idx3] : 440.0f * powf(2.0f, (float)(midiNote + 28 - 69) / 12.0f);
    float f4 = (idx4 < 100) ? HZS[idx4] : f1 * 3.0f;
    
    float f_sub = f1 / 2.0f;
    if (baseIdx >= 12 && (baseIdx - 12) < 100) {
        f_sub = HZS[baseIdx - 12];
    }

    for (int n = 0; n < numSamples; ++n) {
        float angle = 2.0f * PI * (float)n / (float)DISCR;
        float envelope = (cosf(angle) + 1.0f) / 2.0f;
        
        float s = (sinf(angle * f_sub) * 0.6f + 
                   sinf(angle * f1) + 
                   sinf(angle * f2) * 0.7f + 
                   sinf(angle * f3) * 0.5f + 
                   sinf(angle * f4) * 0.4f) * envelope * AMPLITUDE;
        
        samples.push_back(s);
    }
    return samples;
}

void MixAudio(short* outputBuffer, int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        float mixedSample = 0.0f;

        for (int v = 0; v < MAX_VOICES; ++v) {
            if (activeVoices[v].soundIdx != -1) {
                const auto& soundData = precomputedSounds[activeVoices[v].soundIdx];
                size_t offset = activeVoices[v].sampleOffset;

                if (offset < soundData.size()) {
                    mixedSample += soundData[offset];
                    activeVoices[v].sampleOffset++;
                } else {
                    activeVoices[v].soundIdx = -1;
                }
            }
        }

        if (mixedSample > 1.0f) mixedSample = 1.0f;
        if (mixedSample < -1.0f) mixedSample = -1.0f;

        short pcm = (short)(mixedSample * 32767.0f);
        outputBuffer[i * 2] = pcm;     
        outputBuffer[i * 2 + 1] = pcm; 
    }
}

void AddNoteToMixer(int soundIdx) {
    for (int v = 0; v < MAX_VOICES; ++v) {
        if (activeVoices[v].soundIdx == -1) {
            activeVoices[v].soundIdx = soundIdx;
            activeVoices[v].sampleOffset = 0;
            return;
        }
    }

    int oldestVoiceIdx = 0;
    size_t maxOffset = 0;

    for (int v = 0; v < MAX_VOICES; ++v) {
        if (activeVoices[v].sampleOffset > maxOffset) {
            maxOffset = activeVoices[v].sampleOffset;
            oldestVoiceIdx = v;
        }
    }

    activeVoices[oldestVoiceIdx].soundIdx = soundIdx;
    activeVoices[oldestVoiceIdx].sampleOffset = 0;
}

void PlayNext() {
    if (multilang_melodies[selectedIdx].melodyData == nullptr) return;
    const std::vector<BYTE>& melody = *(multilang_melodies[selectedIdx].melodyData);
    
    if (melody.empty()) return;

    if (currentNoteIdx < melody.size()) {
        BYTE note = melody[currentNoteIdx];
        int soundIdx = (int)note - 40;
        if (soundIdx >= 0 && soundIdx < (int)precomputedSounds.size()) {
            AddNoteToMixer(soundIdx);
            currentNoteIdx++;
        }
    } else {
        currentNoteIdx = 0;
    }
}

void CALLBACK TimerCallback(UINT uID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
    if (!hWaveOut) return;

    if (waveHdr1.dwFlags & WHDR_DONE) {
        MixAudio((short*)waveHdr1.lpData, BUFFER_SAMPLES);
        waveOutWrite(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
    }
    if (waveHdr2.dwFlags & WHDR_DONE) {
        MixAudio((short*)waveHdr2.lpData, BUFFER_SAMPLES);
        waveOutWrite(hWaveOut, &waveHdr2, sizeof(WAVEHDR));
    }
}

void RenderUI(HWND hwnd, HDC hdc) {
    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
    
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 0, 0));
    Gdiplus::SolidBrush darkBrush(Gdiplus::Color(255, 0, 0, 0));
    Gdiplus::SolidBrush grayBrush(Gdiplus::Color(255, 120, 120, 120));
    Gdiplus::SolidBrush activeBrush(Gdiplus::Color(255, 0, 120, 255));
    Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255, 255));

    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::StringFormat centerFormat;

    centerFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    centerFormat.SetFormatFlags(0);
    centerFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::FontFamily pianoFamily(L"Segoe UI Symbol");
    Gdiplus::Font pianoFont(&pianoFamily, 72.0f, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::RectF pianoRect(0.0f, 10.0f, 700.0f, 90.0f);
    Gdiplus::SolidBrush pianoBrush(Gdiplus::Color(255, 100, 0, 0));
    const wchar_t* pianoIcon = L"\xD83C\xDFB9";
    graphics.DrawString(pianoIcon, -1, &pianoFont, pianoRect, &centerFormat, &pianoBrush);

    Gdiplus::Font codeFont(&fontFamily, 12.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);

    for (int i = 0; i < 14; ++i) {
        float x = 30.0f;
        float y = 90.0f + (i * 46.0f);
        float w = 60.0f;
        float h = 36.0f;
        float h3 = h / 3.0f;

        Gdiplus::Pen borderPen((i == currentLang) ? Gdiplus::Color(255, 0, 120, 255) : Gdiplus::Color(255, 220, 220, 220), 2.0f);
        graphics.DrawRectangle(&borderPen, x - 2, y - 2, w + 4, h + 4);

        if (i == 0) {
            Gdiplus::SolidBrush red(Gdiplus::Color(255, 218, 41, 28));
            Gdiplus::SolidBrush blue(Gdiplus::Color(255, 0, 57, 166));
            graphics.FillRectangle(&whiteBrush, x, y, w, h3);
            graphics.FillRectangle(&blue, x, y + h3, w, h3);
            graphics.FillRectangle(&red, x, y + h3 * 2, w, h3);
        }
        else if (i == 1) {
            Gdiplus::SolidBrush red(0xFFB22234), blue(0xFF3C3B6E);
            graphics.FillRectangle(&whiteBrush, x, y, w, h);
            for(int j=0; j<7; ++j) if(j%2==0) graphics.FillRectangle(&red, x, y + j*(h/7.0f), w, h/7.0f);
            graphics.FillRectangle(&blue, x, y, w*0.45f, h*0.55f);
        }
        else if (i == 2) {
            Gdiplus::SolidBrush red(0xFFAA151B), yellow(0xFFF1BF00);
            graphics.FillRectangle(&red, x, y, w, h3);
            graphics.FillRectangle(&yellow, x, y + h3, w, h3);
            graphics.FillRectangle(&red, x, y + h3 * 2, w, h3);
        }
        else if (i == 3) {
            Gdiplus::SolidBrush green(0xFF009246), red(0xFFCE2B37);
            float w3 = w / 3.0f;
            graphics.FillRectangle(&green, x, y, w3, h);
            graphics.FillRectangle(&whiteBrush, x + w3, y, w3, h);
            graphics.FillRectangle(&red, x + w3 * 2, y, w3, h);
        }
        else if (i == 4) {
            Gdiplus::SolidBrush black(0xFF000000), red(0xFFFF0000), gold(0xFFFFCC00);
            graphics.FillRectangle(&black, x, y, w, h3);
            graphics.FillRectangle(&red, x, y + h3, w, h3);
            graphics.FillRectangle(&gold, x, y + h3 * 2, w, h3);
        }
        else if (i == 5) {
            Gdiplus::SolidBrush blue(0xFF0055A5), red(0xFFEF4135);
            float w3 = w / 3.0f;
            graphics.FillRectangle(&blue, x, y, w3, h);
            graphics.FillRectangle(&whiteBrush, x + w3, y, w3, h);
            graphics.FillRectangle(&red, x + w3 * 2, y, w3, h);
        }
        else if (i == 6) {
            Gdiplus::SolidBrush red(0xFFBC002D);
            graphics.FillRectangle(&whiteBrush, x, y, w, h);
            graphics.FillEllipse(&red, (float)(x + (w - 18.0f) / 2.0f), (float)(y + (h - 18.0f) / 2.0f), 18.0f, 18.0f);
        }
        else if (i == 7) {
            Gdiplus::SolidBrush red(0xFFDE2110), gold(0xFFFFDE00);
            graphics.FillRectangle(&red, x, y, w, h);
            graphics.FillEllipse(&gold, (float)(x + 4.0f), (float)(y + 4.0f), 6.0f, 6.0f);
        }
        else if (i == 8) {
            Gdiplus::SolidBrush saffron(0xFFFF9933), green(0xFF138808), navy(0xFF000080);
            graphics.FillRectangle(&saffron, x, y, w, h3);
            graphics.FillRectangle(&whiteBrush, x, y + h3, w, h3);
            graphics.FillRectangle(&green, x, y + h3 * 2, w, h3);
            graphics.FillEllipse(&navy, (float)(x + (w - 8.0f) / 2.0f), (float)(y + (h - 8.0f) / 2.0f + 1.0f), 8.0f, 8.0f);
        }
        else if (i == 9) {
            Gdiplus::SolidBrush green(0xFF006600), red(0xFFFF0000);
            graphics.FillRectangle(&green, x, y, w * 0.4f, h);
            graphics.FillRectangle(&red, x + w * 0.4f, y, w * 0.6f, h);
            Gdiplus::SolidBrush gold(0xFFFFCC00);
            graphics.FillEllipse(&gold, (float)(x + w * 0.4f - 6.0f), (float)(y + (h - 12.0f) / 2.0f), 12.0f, 12.0f);
        }
        else if (i == 10) {
            Gdiplus::SolidBrush red(0xFFE30A17), white(0xFFFFFFFF);
            graphics.FillRectangle(&red, x, y, w, h);
            graphics.FillEllipse(&white, x + 14.0f, y + 8.0f, 20.0f, 20.0f);
            graphics.FillEllipse(&red, x + 19.0f, y + 8.0f, 20.0f, 20.0f);
            graphics.FillEllipse(&white, x + 33.0f, y + 15.0f, 6.0f, 6.0f);
        }
        else if (i == 11) {
            Gdiplus::SolidBrush red(0xFFFF0000);
            graphics.FillRectangle(&red, x, y, w, h / 2.0f);
            graphics.FillRectangle(&whiteBrush, x, y + h / 2.0f, w, h / 2.0f);
        }
        else if (i == 12) {
            Gdiplus::SolidBrush red(0xFFCE1126), black(0xFF000000), gold(0xFFC09300);
            graphics.FillRectangle(&red, x, y, w, h3);
            graphics.FillRectangle(&whiteBrush, x, y + h3, w, h3);
            graphics.FillRectangle(&black, x, y + h3 * 2, w, h3);
            graphics.FillEllipse(&gold, x + (w - 6.0f) / 2.0f, y + h3 + 3.0f, 6.0f, 6.0f);
        }
        else if (i == 13) {
            Gdiplus::SolidBrush blue(0xFF0038A8);
            graphics.FillRectangle(&whiteBrush, x, y, w, h);
            graphics.FillRectangle(&blue, x, y + 3.0f, w, 4.0f);
            graphics.FillRectangle(&blue, x, y + h - 7.0f, w, 4.0f);
            graphics.FillEllipse(&blue, x + (w - 10.0f) / 2.0f, y + (h - 10.0f) / 2.0f, 10.0f, 10.0f);
        }
    }
    
    Gdiplus::Font subFont(&fontFamily, 24.0f, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::RectF subRect(125.0f, 100.0f, 450.0f, 40.0f);
    graphics.DrawString(LANGS[currentLang].selectLbl, -1, &subFont, subRect, &centerFormat, &darkBrush);
    
    Gdiplus::Font hintFont(&fontFamily, 24.0f, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);
    
    Gdiplus::RectF hintRect(125.0f, 650.0f, 450.0f, 80.0f); 
    
    graphics.DrawString(LANGS[currentLang].hintLbl, -1, &hintFont, hintRect, &centerFormat, &darkBrush);


}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {

         case WM_CREATE: {
            hComboBox = CreateWindowW(L"COMBOBOX", nullptr, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
            120, 140, 450, 700, hwnd, (HMENU)ID_COMBOBOX, GetModuleHandle(nullptr), nullptr);
            HFONT hFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hComboBox, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            for (const auto& m : multilang_melodies) {
                SendMessageW(hComboBox, CB_ADDSTRING, 0, (LPARAM)m.names[currentLang]);
            }
            SendMessageW(hComboBox, CB_SETCURSEL, 0, 0);

            hBtnPlay = CreateWindowW(L"BUTTON", LANGS[currentLang].playBtn, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 200, 450, 120, hwnd, (HMENU)ID_PLAY_BTN, GetModuleHandle(nullptr), nullptr);

            HFONT hBtnFont = CreateFontW(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hBtnPlay, WM_SETFONT, (WPARAM)hBtnFont, TRUE);

            hBtnReset = CreateWindowW(L"BUTTON", LANGS[currentLang].resetBtn, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                250, 430, 200, 50, hwnd, (HMENU)ID_RESET_BTN, GetModuleHandle(nullptr), nullptr);
            HFONT hBtnResetFont = CreateFontW(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");
            SendMessageW(hBtnReset, WM_SETFONT, (WPARAM)hBtnResetFont, TRUE); 

            HWND langButtons[9];
            HFONT hLangFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Segoe UI Emoji");
            
            break;
        }       
		
        case WM_LBUTTONDOWN: {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            if (mouseX >= 28 && mouseX <= 92) {
                for (int i = 0; i < 14; ++i) {
                    int startY = 90 + (i * 46);
                    int endY = startY + 36;

                    if (mouseY >= startY && mouseY <= endY) {
                        currentLang = i;
                        
                        SendMessageW(hComboBox, CB_RESETCONTENT, 0, 0);
                        for (const auto& item : multilang_melodies) {
                            SendMessageW(hComboBox, CB_ADDSTRING, 0, (LPARAM)item.names[currentLang]);
                        }
                        SendMessageW(hComboBox, CB_SETCURSEL, selectedIdx, 0);

                        SetWindowTextW(hBtnPlay, LANGS[currentLang].playBtn);
                        SetWindowTextW(hBtnReset, LANGS[currentLang].resetBtn);

                        InvalidateRect(hwnd, nullptr, TRUE);
                        SetFocus(hwnd);
                        break;
                    }
                }
            }
            break;
        }		
		
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);

            if (wmId == ID_COMBOBOX && wmEvent == CBN_SELCHANGE) {
                selectedIdx = SendMessageW((HWND)lParam, CB_GETCURSEL, 0, 0);
                currentNoteIdx = 0;
                SetFocus(hwnd);
            }
            if (wmId == ID_PLAY_BTN) { PlayNext(); SetFocus(hwnd); }
            if (wmId == ID_RESET_BTN) { currentNoteIdx = 0; SetFocus(hwnd); }

            if (wmId >= 2000 && wmId <= 2013) { 
                currentLang = wmId - 2000;

                SendMessageW(hComboBox, CB_RESETCONTENT, 0, 0);
                for (const auto& item : multilang_melodies) {
                    SendMessageW(hComboBox, CB_ADDSTRING, 0, (LPARAM)item.names[currentLang]);
                }
                SendMessageW(hComboBox, CB_SETCURSEL, selectedIdx, 0);

                SetWindowTextW(hBtnPlay, LANGS[currentLang].playBtn);
                SetWindowTextW(hBtnReset, LANGS[currentLang].resetBtn);

                InvalidateRect(hwnd, nullptr, TRUE); 
                SetFocus(hwnd);
            }
            break;
        }
        case WM_KEYDOWN: {
            if (wParam == VK_SPACE) PlayNext();
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            HBRUSH hBgBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &clientRect, hBgBrush);
            DeleteObject(hBgBrush);
            RenderUI(hwnd, hdc);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            if (hTimer) {
                timeKillEvent(hTimer);
                hTimer = 0;
            }
            if (hWaveOut) {
                waveOutReset(hWaveOut);
                waveOutUnprepareHeader(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
                waveOutUnprepareHeader(hWaveOut, &waveHdr2, sizeof(WAVEHDR));
                waveOutClose(hWaveOut);
                hWaveOut = nullptr;
            }
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

precomputedSounds.reserve(60);
for (BYTE midiNote = 40; midiNote < 100; ++midiNote) {
precomputedSounds.push_back(GenerateNoteData(midiNote));
}

WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 2, DISCR, DISCR * 4, 4, 16, 0 };
waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
waveHdr1.lpData = (LPSTR)audioBuffer1; waveHdr1.dwBufferLength = BUFFER_SAMPLES * 4;
waveHdr2.lpData = (LPSTR)audioBuffer2; waveHdr2.dwBufferLength = BUFFER_SAMPLES * 4;
waveOutPrepareHeader(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
waveOutPrepareHeader(hWaveOut, &waveHdr2, sizeof(WAVEHDR));

waveOutWrite(hWaveOut, &waveHdr1, sizeof(WAVEHDR));
waveOutWrite(hWaveOut, &waveHdr2, sizeof(WAVEHDR));

hTimer = timeSetEvent(10, 10, TimerCallback, 0, TIME_PERIODIC);

ULONG_PTR gpToken;
Gdiplus::GdiplusStartupInput gpInput;
Gdiplus::GdiplusStartup(&gpToken, &gpInput, nullptr);

WNDCLASSW wc = {};
wc.hInstance = hInstance;
wc.lpszClassName = L"PianoAppClass";
wc.lpfnWndProc = WndProc;
wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(1));
RegisterClassW(&wc);

RECT r = { 0, 0, 700, 800 };
AdjustWindowRect(&r, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
HWND hwnd = CreateWindowExW(0, L"PianoAppClass", L"Piano",WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, nullptr, nullptr, hInstance, nullptr);

MSG msg = {};
while (GetMessageW(&msg, nullptr, 0, 0)) {
TranslateMessage(&msg);
DispatchMessageW(&msg);
}

Gdiplus::GdiplusShutdown(gpToken);
return (int)msg.wParam;
}
