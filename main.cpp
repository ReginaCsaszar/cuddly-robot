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
                  table(new char[size]), playerTable(new char[size]), exploded(false) { fillTables(); }

        virtual ~Minesweeper() {
            delete[] table;
            delete[] playerTable;
        }

0
        void printTable() const {
            std::cout <<"\nTable:\n";
            for (int i = 0; i <= width; ++i) { std::cout << "--"; }
            std::cout << "-\n";
            for (int y = 0; y < height; y++) {
                std::cout << "| ";
                for(int x = 0; x < width; x++) { std::cout << playerTable[y*width+x] <<" "; }
                std::cout << "|\n";
            }
            for (int i = 0; i <= width; ++i) { std::cout << "--"; }
            std::cout << "-";
        }

        bool isExploded() const { return exploded; }

        void reveal(size_t x, size_t y) {

            size_t index = getCoordinate(x, y);
            char *field = &table[index];

            if ( *field == mine) {
                playerTable[index] = mine;
                exploded = true;
                std::cout <<"\nBOOOM! YOU DIED!\n";
            } else if ( *field == '0') {
                playerTable[index] = space;
                revealNeighbors(x, y);
            } else {
                playerTable[index] = *field;
            }
            printTable();
        }

    private:

        void fillTables() {

            memset(table, zero, size);
            //5memset(playerTable, '.', size);

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

            size_t x = index%width;
            size_t y = (index >= width) ? (index-x)/width : 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (!isInside(x+j, y+i)) continue;
                    size_t neighbor = getCoordinate(x+j, y+i);
                    if (isMine(neighbor)) continue;
                    setField(neighbor);
                }
            }
        }

        void setField(size_t index) { table[index] = table[index] - '0' + 1 + '0'; }

        bool isMine(size_t index) { return table[index] == mine; }

        size_t getCoordinate(size_t x, size_t y){ return y*width+x; }

        bool isInside(size_t x, size_t y) { return (x >= 0)&&(x < width)&&(y >= 0)&&(y<width); }

        void revealNeighbors(size_t x, size_t y) {

            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    if (!isInside(x+j, y+i)) continue;
                    size_t neighbor = getCoordinate(x+j, y+i);
                    if (isMine(neighbor)) continue;
                    if (table[neighbor] == zero) {
                        if (playerTable[neighbor] != space) {
                            playerTable[neighbor] = space;
                            revealNeighbors(x+j, y+i);
                            }
                    } else {
                        playerTable[neighbor] = table[neighbor];
                    }
                }
            }
        }

        const size_t width, height, size;
        const int mines;
        bool exploded;
        char* table;
        char* playerTable;
    };
}

int main() {
    std::cout <<"\nWelcome!";
    try {
        Minesweeper ms(10, 10, 15);
        size_t x, y;

        do {
            std::cout <<"\n\nx: ";
            std::cin >> x;
            std::cout <<"y: ";
            std::cin >> y;
            ms.reveal(x, y);
        } while (!ms.isExploded());

    } catch (const std::bad_alloc &e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}