#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

struct ScoreEntry
{
    char name[10];
    int score;
};

struct Scoring
{
    int count;
    ScoreEntry entries[10];
};

int main()
{
    {
        Scoring scoring;

        scoring.count = 2;
        strcpy(scoring.entries[0].name, "RIK");
        scoring.entries[0].score = 100;
        strcpy(scoring.entries[1].name, "ABC");
        scoring.entries[1].score = 200;

        std::ofstream myfile;
        myfile.open("scoring.bin", std::ios::binary);
        myfile.write(reinterpret_cast<char*>(&scoring), sizeof(Scoring));
        myfile.close();
    }

    {
        Scoring scoring;
        std::ifstream myfile;

        myfile.open("scoring.bin", std::ios::binary);
        myfile.read(reinterpret_cast<char*>(&scoring), sizeof(Scoring));
        myfile.close();

        for (int i = 0; i < scoring.count; ++i)
        {
            printf("[%d] %s: %d\n", i+1, scoring.entries[i].name, scoring.entries[i].score);
        }

        // Read from file
        // Show info
    }

    return 0;
}