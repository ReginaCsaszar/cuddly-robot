#include <iostream>
#include <random>
#include <cstring>

namespace {

    const char mine = '*';
    const char space = ' ';

    class Minesweeper {

    public:
        Minesweeper(const size_t width, const size_t height, const int bomb)
                : width(width), height(height), mines(bomb),
                  table(new char[width * height]), playerTable(new char[width * height]) {
            fillTable();
        }

        virtual ~Minesweeper() {
            delete[] table;
            delete[] playerTable;
        }

        void printTable() const {
            for (int y = 0; y < height; y++) {
                for(int x = 0; x < width; x++) { std::cout << table[y*width+x] <<" "; }
                std::cout << std::endl;
            }
        }

        void reveal(int x, int y) {

            int index = y*width+x;
            char *field = &table[index];

            if ( *field == mine) {
                std::cout <<"\nBOOOM! YOU DIED!"<<std::endl;

            } else if ( *field == '0') {
                char *revealTable = new char[width * height];

                revealTable[index] = *field;
                revealNeighboors(x, y);

                std::cout<<"\nThe reveal table content:\n\n";
                for(int y = 0; y < height; y++) {
                    for(int x = 0; x < width; x++) {
                        std::cout <<revealTable[y*width+x]<<" "; }
                    std::cout<<std::endl; }
                delete[] revealTable;

            } else {
                std::cout <<"You revealed a number "<< *field <<std::endl;
            }


        }

    private:

        void fillTable() {

            size_t size = width * height;

            memset(table, space, size);

            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<size_t> randIndex(0, size);

            unsigned mineCount = 0;

            do {
                size_t place = randIndex(gen);
                if(table[place] != mine) {
                    table[place] = mine;
                    mineCount++;
                }
            } while (mineCount < mines);

            countNeighbours();
        }

        void countNeighbours() {

            for(int y = 0; y < height; y++) {

                for(int x = 0; x < width; x++) {

                    if (table[y*width+x] != mine) {

                        int mineCount = 0;
                        if (table[(y-1)*width+x] == mine) { mineCount++;}
                        if (table[(y+1)*width+x] == mine) { mineCount++;}

                        if (x > 0) {
                            if (table[(y-1)*width+x-1] == mine) { mineCount++;}
                            if (table[y*width+x-1] == mine) { mineCount++;}
                            if (table[(y+1)*width+x-1] == mine) { mineCount++;}
                        }

                        if (x < width-1) {
                            if (table[(y-1)*width+x+1]== mine) { mineCount++ ;}
                            if (table[y*width+x+1]== mine) { mineCount++ ;}
                            if (table[(y+1)*width+x+1]== mine) { mineCount++ ;}
                        }

                        table[y*width+x] = mineCount +'0';
                    }
                }
            }
        }

        void revealNeighboors(size_t x, size_t y) {

            if ((table[(y-1)*width+x] == space)&&(playerTable[(y-1)*width+x] != space)) {
                playerTable[(y-1)*width+x] = space;
                revealNeighboors(x, y-1);
            }
            if ((table[(y+1)*width+x] == space)&&(playerTable[(y+1)*width+x] != space)) {
                playerTable[(y+1)*width+x] = space;
                revealNeighboors(x, y+1);
            }

            if (x > 0) {
                if ((table[(y-1)*width+x-1] == space)&&(playerTable[(y-1)*width+x-1] != space)) {
                    playerTable[(y-1)*width+x-1] = space;
                    revealNeighboors(x-1, y-1);
                }
                if ((table[y*width+x-1] == space)&&(playerTable[y*width+x-1] != space)) {
                    playerTable[y*width+x-1] = space;
                    revealNeighboors(x-1, y);
                }
                if ((table[(y+1)*width+x-1] == space)&&(playerTable[(y+1)*width+x-1] != space)) {
                    playerTable[(y+1)*width+x-1] = space;
                    revealNeighboors(x-1, y+1);
                }
            }

            if (x < width-1) {
                if ((table[(y-1)*width+x+1]== space)&&(playerTable[(y-1)*width+x+1] != space)) {
                    playerTable[(y-1)*width+x+1]= space;
                    revealNeighboors(x+1, y-1);
                }
                if ((table[y*width+x+1]== space)&&(playerTable[y*width+x+1] != space)) {
                    playerTable[y*width+x+1] = space;
                    revealNeighboors(x+1, y);
                }
                if ((table[(y+1)*width+x+1]== space)&&(playerTable[(y+1)*width+x+1] != space)) {
                    playerTable[(y+1)*width+x+1] = space;
                    revealNeighboors(x+1, y+1);

                }
            }
        }

        const size_t width, height;
        const int mines;
        char* table;
        char* playerTable;
    };
}

int main() {
    try {
        Minesweeper ms(10, 10, 15);
        ms.printTable();
    } catch (const std::bad_alloc &e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}