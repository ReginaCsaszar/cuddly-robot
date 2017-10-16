#include <iostream>
#include <random>

namespace {

    char mine = '*';
    char empty = '.';
    char space = '0';

    class Minesweeper {
    public:
        Minesweeper(const size_t width, const size_t height, const int bomb)
                : width(width), height(height), mines(bomb),
                  table(new char[width * height]) {
            fillTable();
        }

        /* In a real implementation there would also be a
         * - copy constructor
         * - assignment operator
         * - move constructor
         * - move assignment operator
         * We will learn about them later
         */



        virtual ~Minesweeper() {
            delete[] table;
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

        void printTable() const {
            std::cout<<"\nThe table content:\n\n";
            for(int y = 0; y < height; y++)
            {
                for(int x = 0; x < width; x++)
                {
                    std::cout <<table[y*width+x]<<" ";
                }
                std::cout<<std::endl;
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
                revealNeighboors(x, y, revealTable);

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

            for(int i = 0; i < height*width; i++) {
                table[i] = empty;
            }

            std::random_device rd;
            std::mt19937_64 gen(rd());
            std::uniform_int_distribution<size_t> randIndex(0, width*height);

            for(int i = 0; i < mines; i++) {
                size_t m = randIndex(gen);
                if(table[m] == mine) {
                    i--;
                } else {
                    table[m] = mine; }
            }
        }

        void revealNeighboors(size_t x, size_t y, char *revealTable) {

            if ((table[(y-1)*width+x] == space)&&(revealTable[(y-1)*width+x] != space)) {
                revealTable[(y-1)*width+x] = space;
                revealNeighboors(x, y-1, revealTable);
            }
            if ((table[(y+1)*width+x] == space)&&(revealTable[(y+1)*width+x] != space)) {
                revealTable[(y+1)*width+x] = space;
                revealNeighboors(x, y+1, revealTable);
            }

            if (x > 0) {
                if ((table[(y-1)*width+x-1] == space)&&(revealTable[(y-1)*width+x-1] != space)) {
                    revealTable[(y-1)*width+x-1] = space;
                    revealNeighboors(x-1, y-1, revealTable);
                }
                if ((table[y*width+x-1] == space)&&(revealTable[y*width+x-1] != space)) {
                    revealTable[y*width+x-1] = space;
                    revealNeighboors(x-1, y, revealTable);
                }
                if ((table[(y+1)*width+x-1] == space)&&(revealTable[(y+1)*width+x-1] != space)) {
                    revealTable[(y+1)*width+x-1] = space;
                    revealNeighboors(x-1, y+1, revealTable);
                }
            }

            if (x < width-1) {
                if ((table[(y-1)*width+x+1]== space)&&(revealTable[(y-1)*width+x+1] != space)) {
                    revealTable[(y-1)*width+x+1]= space;
                    revealNeighboors(x+1, y-1, revealTable);
                }
                if ((table[y*width+x+1]== space)&&(revealTable[y*width+x+1] != space)) {
                    revealTable[y*width+x+1] = space;
                    revealNeighboors(x+1, y, revealTable);
                }
                if ((table[(y+1)*width+x+1]== space)&&(revealTable[(y+1)*width+x+1] != space)) {
                    revealTable[(y+1)*width+x+1] = space;
                    revealNeighboors(x+1, y+1, revealTable);

                }
            }
        }

        const size_t width, height;
        const int mines;
        char *table;
    };
}

int main() {
    try {
        Minesweeper ms(10, 10, 15);
        ms.printTable();
        ms.countNeighbours();
        ms.printTable();
        ms.reveal(5, 5);
    } catch (const std::bad_alloc &e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}