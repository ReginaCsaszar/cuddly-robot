#include <iostream>
#include <random>
#include <cstring>

namespace {

    const char mine = '*';
    const char zero = '0';
    const char space = ' ';

    class Minesweeper {

    public:
        Minesweeper(const size_t width, const size_t height, const int bomb)
                : width(width), height(height), mines(bomb), size(width * height),
                  table(new char[size]), playerTable(new char[size]) {
            fillTables();
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

        void fillTables() {

            memset(table, zero, size);
            memset(playerTable, space, size);

            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<size_t> randIndex(0, size);

            unsigned mineCount = 0;

            do {
                size_t place = randIndex(gen);
                if(table[place] != mine) {
                    table[place] = mine;
                    countNeighbours(place);
                    mineCount++;
                }
            } while (mineCount < mines);
        }

        void countNeighbours(size_t index) {

            size_t x = index%width-1;
            size_t y = index >= width ? (index - (x))/width-1 : 0;

            for (int i = 0; i < 3; i++) {
                if (y+i >= height) break;
                if (y+i >= 0) {
                    for (int j = 0; j < 3; j++) {
                        size_t neighbor = getCoordinate(x+j, y+i);
                        if (isMine(neighbor)) continue;
                        if ((x+j >= 0)&&(x+j < width)) setField(neighbor);
                    }
                }
                if ((i == 1)&&(index < (width))) break;
            }
        }

        void setField(size_t index) { table[index] = table[index] - '0' + 1 + '0'; }

        bool isMine(size_t index) { return table[index] == mine; }

        size_t getCoordinate(size_t x, size_t y){ return y*width+x; }

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

        const size_t width, height, size;
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