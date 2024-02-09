//battleship(game) by Stanislav Budzynskyi
//may contain some very rare singleplayer mode bugs that I didn't notice
//clearer code using vectors will be soon
//If you want English language version, please contact me
#include <iostream>
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

using namespace std;

bool showhints = 1, twoplayers, replay = 0, startmessage = 1;
void console_clear(){ //windows only
        system("cls"); // "system("clear");" - linux/apple
    }

int conv(char a){
    switch(a){
        case 'A':
            return 1;
        case 'B':
            return 2;
        case 'C':
            return 3;
        case 'D':
            return 4;
        case 'E':
            return 5;
        case 'F':
            return 6;
        case 'G':
            return 7;
        case 'H':
            return 8;
        case 'I':
            return 9;
        case 'J':
            return 10;
        case 'a':
            return 1;
        case 'b':
            return 2;
        case 'c':
            return 3;
        case 'd':
            return 4;
        case 'e':
            return 5;
        case 'f':
            return 6;
        case 'g':
            return 7;
        case 'h':
            return 8;
        case 'i':
            return 9;
        case 'j':
            return 10;
        default:
            return -1;
    }}

bool check_I(int a, int b){
    if(a<=0 || b<=0 || a>10 || b>10) return 1;
    else return 0;
    }

class Player {
    public:
    int freespace = -1;
    int map[10][10];
    //int health [0][0] - hp_max; [0][1] - is taken; [1][0] - curr hp; [1][+] - position;
    char x_L;
    int x1, x2, y1, y2;
    int **ships_inf = new int*[11];//[0] hp max [1] hp - curr [+] position;
    int pl = 2;
    int ships = 10;
    //initiazing
    Player(){
        for(int i = 0; i<10; ++i){
            ships_inf[i] = new int[10];
        }
        for(int i=0;i<100;++i){
        map[i/10][i%10] = -1;
        }
    }
    //ships
    void map_cr_ships(){
        cout << "    A B C D E F G H I J\n";
        cout << "   ┌────────────────────┐\n";
        for(int i=0; i<10;++i){
            cout << " " << i+1;
            if(i!=9){
                cout << " ";}
            line_get(i);
            cout << endl;
        }
        cout << "   └────────────────────┘\n";
    }

    void dotter(int ship){
        for(int i=2;i<ships_inf[ship][0]*2+2;++i){
            int x = ships_inf[ship][i];
            int y = ships_inf[ship][++i];
            for (int j=-1;j<=1;++j){
                for (int k=-1;k<=1;++k){
                    if(x+k>=0 && x+k<=9 && y+j>=0 && y+j<=9 && map[x+k][y+j]==-1) map[x+k][y+j] = 10;
                }
            }
        }
    }

    void dot_clear(){
        for(int i=0;i<100;++i){
            if(map[i/10][i%10] == 10) map[i/10][i%10] = -1;
        }
    }

    bool check(){
        int len = 0, pos = 0;
        char kierunek;
        if(x1 == x2){
            if(y1>y2) {
                len = y1-y2;
                kierunek = 'G';
            }
            else {
                len = y2-y1;
            kierunek = 'D';
            }
        }
        else if(y1 == y2){
            if(x1>x2) {
                len = x1-x2;
                kierunek = 'L';
            }
            else {
                len = x2-x1;
                kierunek = 'P';
            }
        }
        else return 1;
        if(len>=0 && len<=3){
            for(int i = 0; i<len; ++i) pos+=4-i;
        }
        else return 1;
        freespace = -1;
        for (int i = pos; i < pos + 4 - len; ++i){
            if(ships_inf[i][0] == -1){
                freespace = i;
                break;
                }
        }
        if(freespace == -1) return 1;
        ships_inf[freespace][0] = len+1;
        ships_inf[freespace][1] = len+1;
        switch(kierunek){
            case 'P':
                for(int i = 2;i<=len*2+2;++i){
                    ships_inf[freespace][i] = x1+i/2-1;
                    ships_inf[freespace][++i] = y1;
                    map[x1+i/2-1][y1] = freespace;
                }
                break;
            case 'L':
                for(int i = 2;i<=len*2+2;++i){
                    ships_inf[freespace][i] = x2+i/2-1;
                    ships_inf[freespace][++i] = y1;
                    map[x2+i/2-1][y1] = freespace;
                }
                break;
            case 'G':
                for(int i = 2;i<=len*2+2;++i){
                    ships_inf[freespace][i] = x1;
                    ships_inf[freespace][++i] = y2+i/2-1;
                    map[x1][y2+i/2-1] = freespace;
                }
                break;
            case 'D':
                for(int i = 2;i<=len*2+2;++i){
                    ships_inf[freespace][i] = x1;
                    ships_inf[freespace][++i] = y1+i/2-1;
                    map[x1][y1+i/2-1] = freespace;
                }
                break;
        }
        console_clear();
        return 0;
    }

    void ships_get(){
        if(showhints) cout << "(i) pozycja statku jest wprowadzana przez 2 koordynaty - literę i cyfrę rozdzielone spacją\npo poprawnym wprowadzeniu pirwszej pozycji potrzeba wprowadzić pozycję do której będzie sięgał statek\n(statek może być ustawiony tylko w linii prostej)\nPrzykładowa pozycja do wprowadzenia \"A 8\"\npodpowiedzi można wyłączyć w menu głównym\n";
        for(int i=0; i<10;++i) ships_inf[i][0] = -1;
        for(int i=0; i<10; ++i){
            map_cr_ships();
            y1 = -1;
            x_L = ' ';
            cout << "wprowadź pozycję początku statku\n";
            scanf("%c%d",&x_L, &y1);
            x1 = conv(x_L);
            while(check_I(x1, y1) || map[x1-1][y1-1] != -1){
                console_clear();
                map_cr_ships();
                cout << "Zła pozycja, wprowadź ponownie\n";
                y1 = -1;
                x_L = ' ';
                scanf("%c%d",&x_L, &y1);
                x1 = conv(x_L);
            }
            console_clear();
            --x1;
            --y1;
            map[x1][y1] = 12;
            map_cr_ships();
            cout << "wprowadź pozycję końca statku\n";
            y2 = -1;
            x_L = ' ';
            scanf("%c%d",&x_L, &y2);
            x2 = conv(x_L);
            while(check_I(x2, y2) || (map[x2-1][y2-1] != -1 && map[x2-1][y2-1] != 12) ){
                console_clear();
                map_cr_ships();
                cout << "Zła pozycja, wprowadź ponownie\n";
                y2 = -1;
                x_L = ' ';
                scanf("%c%d",&x_L, &y2);
                x2 = conv(x_L);
            }
            --x2;
            --y2;
            if(check()){
                console_clear();
                map[x1][y1] = -1;
                cout << "(i)statek nie może być umieszczony w taki sposób lub osiągnąłeś limit statków danej długości!\n";
                --i;
                continue;
            }
            else dotter(freespace);
        }
        dot_clear();
    }
    //*ships
    //map
    void line_get(int line){
        cout << "│";
        for (int i=0;i<10;++i){
            switch(map[i][line]){
                case -1:
                    cout << "  ";
                    break;
                case 10:
                    cout << "∙ ";
                    break;
                case 11:
                    cout << "X ";
                    break;
                case 12:
                    cout << "# ";
                    break;
                default:
                    cout << "██";
                    break;
            }
        }
        cout << "│";
    }

    void line_get_secure(int line){
        cout << "│";
        for (int i=0;i<10;++i){
            switch(map[i][line]){
                case 10:
                    cout << "∙ ";
                    break;
                case 11:
                    cout << "X ";
                    break;
                default:
                    cout << "  ";
                    break;
            }
        }
        cout << "│";
    }
    //*map
    //attack/defend
    int attack(bool wrongPos){
        cout << "\nRuch Gracza " << pl << "\n\nWpisz pozycję\n";
        if(wrongPos) cout << "Zła pozycja, wprowadź ponownie\n";
        y1 = -1;
        x_L = ' ';
        scanf("%c%d",&x_L, &y1);
        x1 = conv(x_L);
        --x1;
        --y1;
        if(check_I(x1+1, y1+1) || map[x1][y1] == 10 || map[x1][y1] == 11){
            return -1;
        }
        else{
            int pos = x1*10+y1;
            return pos;
        }
    }

    bool check_att(int x,int y){
        if (map[x][y] != 10 && map[x][y] != 11) return 0;
        else return 1;
    }

    int get_attack(int x,int y){
        if (map[x][y] == -1) {
            map[x][y] = 10;
            return 0;
        }
        else {
            /*if(map[x][y]>=10){
                cout << x << " " << y << "\n";
                Sleep(10000);
            }*/
            if(ship_destroy(x, y)) return 2;
            else return 1;
        }
    }

    bool ship_destroy(int x, int y){
        int O_O = map[x][y];
        map[x][y] = 11;
        --ships_inf[O_O][1];
        if(ships_inf[O_O][1] == 0){
            --ships;
            dotter(O_O);
            return 1;
        }
        return 0;
    }
    //*attack/defend
    //1p mode
    int s_count(){
        int am = 0;
        for(int x = 0;x<10;++x){
            for(int y = 0;y<10;++y){
                if(map[x][y] >=0 && map[x][y] <=3 && (x%2+y)%2==0) ++am;
            }
        }
        return am;
    }
};

class Player2 : public Player{
	public:
    int *priority_first = new int[5], **priority_second = new int*[11], **priority_third = new int*[11];
    bool isattack = 0;
    Player2(){
        pl = 1;
        ships_inf[10] = new int[10];
        for(int i = 0; i<=10; ++i){
            priority_second[i] = new int[11];
            priority_third[i] = new int[11];
        }
        for(int i=0;i<100;++i){
        map[i/10][i%10] = -1;
        }
    }

    void is2p(){
        if(!twoplayers) ships_generate();
        else {
            cout << "Gracz 2 wprowadza statki\n\n";
            ships_get();
        }
    }
    //automatic ships generation
    void ships_generate(){
        for(int i=0; i<10;++i) ships_inf[i][0] = -1;
        for(int i=0;i<=10;++i) {
            for(int j = 0; j<=10;++j) priority_third[i][j] = -1;
            priority_second[0][i] = i;
            priority_second[i][0] = 10;
        }
        for(int i=1;i<=10;++i){
            for(int j=0;j<10;++j){
                priority_second[i][j+1] = (i-1)*10+j;
            }
        }
        int ship_len = 4, amount = 1;
        for(int i=0;i<10;++i){
            x1 = rand()%priority_second[0][0]+1;
            y1 = rand()%priority_second[priority_second[0][x1]][0]+1;
            x1 = priority_second[priority_second[0][x1]][y1];
            y1 = x1%10;
            x1 /=10;
            pos2_gen(ship_len);
            if(priority_first[0]==0){
                --i;
                continue;
            }
            while(priority_first[0]>0){
                int pos = rand()%priority_first[0]+1;
                y2 = priority_first[pos]%10;
                x2 = priority_first[pos]/10;
                priority_first[pos] = -1;
                sorter();
                if(check()){
                    if(priority_first[0]==0){
                        --i;
                        continue;
                    }
                }
                else{
                    dotter2(freespace);
                    priority_first[0] = 0;
                    for(int i = 1; i<5;++i)priority_first[i] = -1;
                    --amount;
                    if(amount==0){
                        --ship_len;
                        amount = 5-ship_len;
                    }
                    continue;
                }
            }
        }
        dot_clear();
        isattack = 1;
    }

    void dotter2(int ship){
        for(int i=2;i<ships_inf[ship][0]*2+2;++i){
            int x = ships_inf[ship][i];
            int y = ships_inf[ship][++i];
            for (int j=-1;j<=1;++j){
                for (int k=-1;k<=1;++k){
                    if(x+k>=0 && x+k<=9 && y+j>=0 && y+j<=9 && map[x+k][y+j]==-1 && twoplayers)map[x+k][y+j] = 10;
                    else {
                        if(x+k>=0 && x+k<=9 && y+j>=0 && y+j<=9 && map[x+k][y+j]>=-1 && map[x+k][y+j]<10 && !isattack){
                            if(map[x+k][y+j]==-1)map[x+k][y+j] = 10;
                            map_autodel(x+k, y+j);
                        }
                        if(x+k>=0 && x+k<=9 && y+j>=0 && y+j<=9 && isattack) map_autodel(x+k, y+j);
                    }
                }
            }
        }
    }

    void sorter(){
        int temp = 0;
        for(int i = 1; i<=4;++i) {
            if(priority_first[i]!=-1)++temp;
            if(priority_first[i]==-1 && i!=4){
                priority_first[i] = priority_first[i+1];
                priority_first[i+1] = -1;
            }
        }
        priority_first[0] = temp;
    }

    void info(){
        for(int i = 0; i<=10;++i){
            for(int j = 0; j<=10;++j){
                cout << i << " " << j << ": " << priority_second[i][j] << " " << priority_third[i][j] << "\n";
            }
        }
    }

    void map_autodel(int x, int y){
        int x_pos = -1, xl;
        bool sin2 = true;
        //cout << "clearing " << x << " " << y << "\n";
        for(xl=1;xl<=priority_second[0][0];++xl){ //xL, not x1
            if(x == priority_second[priority_second[0][xl]][1]/10) {
                x_pos = priority_second[0][xl];
                break;
            }
        }
        //cout <<  "pos " << x_pos << "\n";
        for(int yl=1;x_pos != -1 && yl<=priority_second[x_pos][0];++yl){
            if(priority_second[x_pos][yl] == x*10+y){
                //cout << "deleted from 2 "<< priority_second[x_pos][yl]<< "\n";
                priority_second[x_pos][yl] = -1;
                --priority_second[x_pos][0];
                sin2=false;
            }
            if(priority_second[x_pos][0] == 0){
                //cout << "                 deleting line " << priority_second[0][xl] << " " << xl<< "\n";
                priority_second[0][xl] = -1;
                for(int i = 1; i<=priority_second[0][0];++i){
                    if(priority_second[0][i] == -1 && i!=10){
                        priority_second[0][i] = priority_second[0][i+1];
                        priority_second[0][i+1] = -1;
                    }
                }
                --priority_second[0][0];
                break;
            }
            if(priority_second[x_pos][yl] == -1 && yl!=10){
                priority_second[x_pos][yl] = priority_second[x_pos][yl+1];
                priority_second[x_pos][yl+1] = -1;
            }
        }
        if(sin2){
        for(xl=1;xl<=priority_third[0][0] ;++xl){ //xL, not x1
            if(x == priority_third[priority_third[0][xl]][1]/10) {
                x_pos = priority_third[0][xl];
                break;
            }
        }
        for(int yl=1;x_pos != -1 && yl<=priority_third[x_pos][0] && x_pos != (-1);++yl){
            if(priority_third[x_pos][yl] == x*10+y){
                //cout << "deleted from 3 "<< priority_third[x_pos][yl] << "\n";
                priority_third[x_pos][yl] = -1;
                --priority_third[x_pos][0];
            }
            if(priority_third[x_pos][0] == 0){
                //cout << "                 deleting line " << priority_third[0][xl] << "\n";
                priority_third[0][xl] = -1;
                for(int i = 1; i<=priority_third[0][0];++i){
                    if(priority_third[0][i] == -1 && i!=10){
                        priority_third[0][i] = priority_third[0][i+1];
                        priority_third[0][i+1] = -1;
                    }
                }
                --priority_third[0][0];
                break;
            }
            if(priority_third[x_pos][yl] == -1 && yl!=10){
                priority_third[x_pos][yl] = priority_third[x_pos][yl+1];
                priority_third[x_pos][yl+1] = -1;
            }
        }
        }
        //for(int i = 0; i<=10;++i) cout << i << ": " << priority_second[0][i] << " " << priority_third[0][i] << "\n";
    }

    void pos2_gen(int len){
        int i = 0;
        len-=1;
        priority_first[0] = 4;
        if(check_I(x1+len+1,y1+1)) --priority_first[0];
        else {
            if(map[x1+len][y1] >= 0 && map[x1+len][y1] <=10) --priority_first[0];
            else {
                ++i;
                priority_first[i] = (x1+len)*10 + y1;
            }
        }
        if(check_I(x1-len+1,y1+1)) --priority_first[0];
        else {
            if(map[x1-len][y1] >= 0 && map[x1-len][y1] <= 10) --priority_first[0];
            else {
                ++i;
                priority_first[i] = (x1-len)*10 + y1;
            }
        }
        if(check_I(x1+1,y1+len+1)) --priority_first[0];
        else {
            if(map[x1][y1+len] >= 0 && map[x1][y1+len] <= 10) --priority_first[0];
            else {
                ++i;
                priority_first[i] = x1*10 + y1+len;
            }
        }
        if(check_I(x1+1,y1-len+1)) --priority_first[0];
        else {
            if(map[x1][y1-len] >= 0 && map[x1][y1-len] <= 10) --priority_first[0];
            else {
                ++i;
                priority_first[i] = x1*10 + y1-len;
            }
        }
    }
    //*automatic ships generation
    //automatic attack
    void attack_gen(int O_O){
        priority_third[0][0] = 10;
        priority_second[0][0] = 10;
        for(int i=1;i<=10;++i) {
            priority_third[0][i] = i;
            priority_third[i][0] = 5;
            for(int j = 6; j<=10;++j) priority_second[i][j] = -1;
            priority_second[0][i] = i;
            priority_second[i][0] = 5;
        }
        for(int x = 0;x<10;++x){
            for(int y = 0;y<10;++y){
                if((x%2+y)%2==0){
                    if(O_O>=2)priority_second[x+1][y/2+1] = x*10+y;
                    else priority_third[x+1][y/2+1] = x*10+y;
                }
                else{
                    if(O_O>=2)priority_third[x+1][y/2+1] = x*10+y;
                    else priority_second[x+1][y/2+1] = x*10+y;
                }
            }
        }
    }

    int attack_auto(){
        /*cout << "first:\n";
        for(int i = 0; i<=4;++i)cout << priority_first[i] << " ";
        cout << "\nsecond:\n";
        for(int i = 1; i<=10;++i){
            cout << i << ": ";
            for(int j = 1; j<priority_second[i][0]+1;++j)cout << priority_second[i][j] << " ";
            cout << "\n";
        }
        cout << "\nthird:\n";
        for(int i = 1; i<=10;++i){
            cout << i << ": ";
            for(int j = 1; j<priority_third[i][0]+1;++j)cout << priority_third[i][j] << " ";
            cout << "\n";
        }*/
        if(priority_first[0]>0){
            int cont = rand()%priority_first[0]+1;
            int pos = priority_first[cont];
            priority_first[cont] = -1;
            sorter();
            return pos;
        }
        else{
            if(priority_second[0][0]>0){
            int pos = rand()%priority_second[0][0]+1;
            int y = rand()%priority_second[priority_second[0][pos]][0]+1;
            pos = priority_second[priority_second[0][pos]][y];
            return pos;
        }
        else{
            if(priority_third[0][0]>0){
            int pos = rand()%priority_third[0][0]+1;
            int y = rand()%priority_third[priority_third[0][pos]][0]+1;
            pos = priority_third[priority_third[0][pos]][y];
            return pos;
        }}}
    }

    void first_del(int pos){
        for(int i=1; i<=priority_first[0];++i){
            if(priority_first[i]==pos){
                priority_first[i] = -1;
                break;
            }
        }
        sorter();
    }

    void ship_cl(){
        for(int i = 0; i<10;++i) ships_inf[10][i]=0;
    }

    void ship_add(int x, int y){
        ++ships_inf[10][0];
        ships_inf[10][ships_inf[10][0]*2] = x;
        ships_inf[10][ships_inf[10][0]*2+1] = y;
    }

    void ship_del(){
        dotter2(10);
        priority_first[0]=0;
        for(int i = 1; i<5;++i)priority_first[i] = -1;
    }

    void ship_check(int p, char o){
        for(int i=1; i<=4;++i){
            if(p != priority_first[i]/10 && o == 'x'){
                /*cout << "deleting " << priority_first[i] << " from 1\n";
                Sleep(1000);*/
                priority_first[i] = -1;
                sorter();
            }
            else {
                if(p != priority_first[i]%10 && o == 'y'){
                /*cout << "deleting " << priority_first[i] << " from 1\n";
                Sleep(1000);*/
                priority_first[i] = -1;
                sorter();
                }
            }
        }
    }
};

class Game{
	Player player;
	Player2 player2;
	bool p1_d = 0, p2_d = 0;
	int pos = 0, x, y, x1, y1;
	public:
	Game(){
		mainmenu();
		}
    //game
    void map_gen(bool end){
        cout << "    A B C D E F G H I J               A B C D E F G H I J\n";
        cout << "   ┌────────────────────┐            ┌────────────────────┐\n";
        for(int i=0; i<10;++i){
            cout << " " << i+1;
            if(i!=9) cout << " ";
            if(!twoplayers && !end){
                player.line_get(i);
                cout << "          " << i+1;
                if(i!=9) cout << " ";
                player2.line_get_secure(i);
                }
            else if(end){
                player.line_get(i);
                cout << "          " << i+1;
                if(i!=9) cout << " ";
                player2.line_get(i);
            }
            else{
                player.line_get_secure(i);
                cout << "          " << i+1;
                if(i!=9) cout << " ";
                player2.line_get_secure(i);
            }
            cout << "\n";
        }
        cout << "   └────────────────────┘            └────────────────────┘\n";
    }

    void game_start(){
        int a = -1, i=1;
        cout << "wpisz ilość graczy(1 lub 2)\n";
        cin >> a;
        while(!(a>=1 && a <=2) || i >=12){
            console_clear();
            cout << "1 lub 2!!!\n";
            if (i>=3 && i<6) cout << "proszę wyszukać w google: \"jak zrozumieć polecenie\"\n";
            if (i>=6 && i<9) cout << "Human...\n";
            if (i>=9 && i<12) cout << "W P I S Z   J E D E N   L U B   D W A   Z A   P O M O C Ą   C Y F E R E K\n";
            if (i==12) cout << "Pozwoliłem sobie wybrać za ciebie\n";
            cin >> a;
            ++i;
            }
        if (i>=12 && !(a>=1 && a <=2)) a = 1;
        if (a==1) twoplayers = 0;
        else if(a == 2) twoplayers = 1;
        console_clear();
        if(twoplayers) cout << "Gracz 1 wprowadza statki\n\n";
        player.ships_get();
        player2.is2p();
        game_main();
    }
    //game main core
    void game_main(){
    bool wrongPos = 0;
    int moves = 0, isturn, firstshot = 0;
    if(showhints) cout << "(i) format pozycji statku do atakowania musi być taki jak w przypadku ustawiania statków\n";
    if(!twoplayers){
        player2.attack_gen(player.s_count());
        player2.ship_cl();
    }
    while(player.ships != 0 && player2.ships != 0){
        isturn = 1;
        while(isturn && player2.ships != 0){
            map_gen(0);
            pos = player2.attack(wrongPos);
            x = pos /10;
            y = pos %10;
            while(pos == -1 || player2.check_att(x, y)){
                wrongPos = 1;
                console_clear();
                map_gen(0);
                pos = player2.attack(wrongPos);
                x = pos /10;
                y = pos %10;
            }
            wrongPos = 0;
            console_clear();
            isturn = player2.get_attack(x, y);
            ++moves;
        }
        if(player2.ships != 0) isturn = 1;
        else isturn = 0;
        while(isturn && player.ships != 0 && twoplayers){
            map_gen(0);
            pos = player.attack(wrongPos);
            x = pos /10;
            y = pos %10;
            while(pos == -1 || player.check_att(x, y)){
                wrongPos = 1;
                console_clear();
                map_gen(0);
                pos = player.attack(wrongPos);
                x = pos /10;
                y = pos %10;
            }
            wrongPos = 0;
            console_clear();
            isturn =  player.get_attack(x, y);
            ++moves;
        }
        while(isturn && player.ships != 0 && !twoplayers){
            pos = player2.attack_auto();
            x = pos/10;
            y = pos%10;
            while(player.map[x][y] >=10){
                player2.map_autodel(x, y);
                pos = player2.attack_auto();
                x = pos/10;
                y = pos%10;
            }
            isturn =  player.get_attack(x, y);
            player2.map_autodel(x, y);
            if(isturn>0){
                player2.ship_add(x,y);
                if(firstshot == 0) firstshot = 1;
                if(isturn == 2){//ship_del
                    firstshot = 0;
                    player2.ship_del();
                    player2.ship_cl();
                }
                else {
                    if(firstshot == 1){
                        firstshot = 2;
                        shot_cr();
                    }
                    else {
                        shot_re_cr();
                    }
                }
            }
            //if(firstshot != 0 && isturn == 0)
            player2.first_del(pos);
            //player2.info();
        }
    }
    console_clear();
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n";
    Sleep(1000);
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n";
    Sleep(1000);
    if(player.ships == 0 && twoplayers) cout << "                    gracz 2 zwyciężył!\n";
    else if (twoplayers) cout << "                    gracz 1 zwyciężył!\n";
    if(player.ships == 0 && !twoplayers) cout << "                       Przegrałeś\n";
    else if (!twoplayers) cout << "                  Udało ci się mnie pokonać!\n";
    Sleep(2000);
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n";
    Sleep(1000);
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n";
    Sleep(2000);
    cout << "                ilość wykonanych ruchów: " << moves << "\n";
    Sleep(1000);
    cout << "'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''\n";
    Sleep(2000);
    map_gen(1);
    Sleep(1000);
    if (twoplayers || player.ships != 0)cout << "\nChcesz zagrac ponownie? (tak/nie)\n";
    else cout << "\nChcesz spróbować jeszcze raz? (tak/nie)\n";
    string a;
    cin >> a;
    if(a == "tak" || a == "Tak" || a == "TAK") {
        replay = 1;
        console_clear();
        startmessage = 0;
    }
    }
    //*game
    //menu
    void mainmenu(){
        string a;
        if(startmessage)cout << "Witam w grze Statki\nŻeby zacząć grę wpisz 'start'\nMasz możliwość zmiany ustawień gry za pomocą polecenia 'config'\nChcesz więcej informacji? Wpisz 'help'\n";
        else{
            cout << "   witam ponownie!\n''''''''''''''''''''\n";
            printhelp_mini();
        }
        while(1){
            bool err = 1;
            cin >> a;
            if(a == "start" || a == "Start" || a == "START"){
                console_clear();
                game_start();
                break;
            }
            if(a == "config" || a == "Config" || a == "CONFIG"){
                err = 0;
                console_clear();
                config();
                printhelp_mini();
            }
            if( a == "help" || a == "Help" || a == "HELP"){
                err = 0;
                console_clear();
                help();
                printhelp_mini();
            }
            if (a == "exit" || a == "Exit" || a == "EXIT"){
                console_clear();
                cout << "\n\nDo zobaczenia chamie jeden...\n\n\n";
                break;
            }
            if (a == "Szymon" || a == "szymon" || a == "SZYMON"){
                console_clear();
                cout << "\n\nUwaga!!! Odporność gry została ZWIĘKSZONA PONAD WSZELKĄ MIARĘn\n\n";
                Sleep(5000);
                console_clear();
                game_start();
                break;
            }

            if(err){
                console_clear();
                cout << "źle wprowadzone polecenie\n";
                printhelp_mini();
            }
        }
    }
    //mini help
    void printhelp_mini(){
    cout << "start - zacząć grę\nhelp - pomoc\nconfig - ustawienia\n";
    }
    //config change
    void config(){
        string a;
        while(1){
            cout << "showhints = " << showhints << " - pomoc w grze\nW celu zmiany wartości wpisz nazwę ustawienia i znaczenie po spacji(1 - włączone, 0 - wyłączone)\nexit - powrót do menu\n";
            bool err = 1;
            cin >> a;
            if(a == "showhints"){
                err = 0;
                cin >> showhints;
                console_clear();
                }
            if (a == "exit") break;
            if(err){
                console_clear();
                cout << "źle wprowadzone polecenie!\n";
            }
        }
        console_clear();
    }
    //game help
    void help(){
        string a;
        string help1 = "Celem gry jest zniszczenie wszystkich statków przeciwnika,\nKażdy gracz posiada 10 statków:\n1 czteromasztowiec\n2 trójmasztowce\n3 dwumasztowce\n4 jednomasztowce \nStatki ustawiane są w pionie lub poziomie, w taki sposób,\naby nie stykały się one ze sobą ani bokami, ani rogami.\n\nprev - poprzednia strona, next - następna\n              strona 1/3\n\n";
        string help2 = "Oznaczenia:\n    A B C D E F G H I J\n   ┌────────────────────┐\n 1 │X ∙           ████  │ ██ - statek\n 2 │∙ ∙                 │\n 3 │                    │ X - trafiony statek\n 4 │        ██        ██│\n 5 │        ██∙         │ ∙ - miejsce, w które strzelał przeciwnik\n 6 │        ██          │\n 7 │∙       ██  ∙       │\n 8 │                    │\n 9 │                    │\n 10│██                  │\n   └────────────────────┘\n         strona 2/3\n\n";
        string help3 = "Polecenia w menu:\nconfig - pozwala zmieniać ustawienia gry żeby bardziej ją dopasować do gracza\nhelp - ponownie wyświetla pomoc\nstart - pozwala rozpocząć bitwę okrętów(bo na tym polega ta gra)\nexit - wyjście z gry(nie polecam)\n\nnext - zamknij samouczek\n              strona 3/3\n\n";
        int page = 1;
        cout << help1 << "\n";
        while(page!=4){
            cin >> a;
            if(a == "prev" && page>1) --page;
                else if(page == 1 && a == "prev") cout << "to jest początek";//yes, it will be cleared before user can see it
                    else ++page;
            switch (page){
                case 1:
                    console_clear();
                    cout << help1;
                    break;
                case 2:
                    console_clear();
                    cout << help2;
                    break;
                case 3:
                    console_clear();
                    cout << help3;
                    break;
            }
        }
        console_clear();
    }
    //*menu
    //1p mode
    void shot_cr(){
    player2.priority_first[0]=0;
    for(int i = 1; i<5;++i)player2.priority_first[i] = -1;
    //cout << "creating P1 at" << pos << "\n";
        if(x+1<10 && player.map[x+1][y]<10){
            ++player2.priority_first[0];
            player2.priority_first[player2.priority_first[0]] = (x+1)*10+y;
        }
        if(x-1>=0 && player.map[x-1][y]<10){
            ++player2.priority_first[0];
            player2.priority_first[player2.priority_first[0]] = (x-1)*10+y;
        }
        if(y+1<10 && player.map[x][y+1]<10){
            ++player2.priority_first[0];
            player2.priority_first[player2.priority_first[0]] = x*10+y+1;
        }
        if(y-1>=0 && player.map[x][y-1]<10){
            ++player2.priority_first[0];
            player2.priority_first[player2.priority_first[0]] = x*10+y-1;
        }
    //cout << "created " << player2.priority_first[0] <<"\n";
    x1=x;
    y1=y;
    }

    void shot_re_cr(){
        if(x1==x){
            player2.ship_check(x, 'x');
            ++player2.priority_first[0];
            if(y+1 < 10 && player.map[x][y+1]<10) {player2.priority_first[player2.priority_first[0]] = x*10+y+1;}
            else {
                if(y-1 >= 0)player2.priority_first[player2.priority_first[0]] = x*10+y-1;}
        }
        else {
            if(y1==y){
                player2.ship_check(y, 'y');
                ++player2.priority_first[0];
                if(x+1 < 10 && player.map[x+1][y]<10) {player2.priority_first[player2.priority_first[0]] = (x+1)*10+y;}
                else {
                    if(x-1 >= 0)player2.priority_first[player2.priority_first[0]] = (x-1)*10+y;}
            }
        }
    }
};

void start(){
    Game game;
}

int main(){
    setlocale(LC_ALL, "pl_PL.UTF-8");
    srand(time(NULL));
    start();
    while(replay){
    	replay = 0;
        start();
    }
    return 0;
}
//fast test lines
//A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 J 4 J 5 J 10 J 9 A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 F 4 F 5 I 10 I 9
//start 2 A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 J 4 J 5 J 10 J 9 A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 F 4 F 5 I 10 I 9 A 1 A 3 A 5 A 7 A 10 B 10 C 10 D 10 C 1 A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 2 C 3 C 5 C 6 C 7 J 1 J 2 F 4 F 5 I 10 I 9
//start 1 A 1 A 1 A 3 A 3 A 5 A 5 A 7 A 7 A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 J 4 J 5 J 10 J 9 a1a2a3a4a5a6a7a8a9a10j1j2j3j4j5j6j7j8j9j10g1g2g3g4g5g6g7g8g9g10b1b2b3b4b5b6b7b8b9b10c1c2c3c4c5c6c7c8c9c10d1d2d3d4d5d6d7d8d9d10e1e2e3e4e5e6e7e8e9e10
//start 1 a2a2a4a4a6a6a8a8A 10 D 10 C 1 C 3 C 5 C 7 J 1 J 2 J 4 J 5 J 10 J 9
