#ifndef WET2_GIT_PLAYERSMANAGER_H
#define WET2_GIT_PLAYERSMANAGER_H
#include "AVLTree.h"
#include "HashTable.h"
#include "Union.h"

class PlayerManager{
public:
    class Player{
        friend class PlayerManager;
        int player_id;
        int group_id;
        int score;
        int level;

    public:
        Player(int player_id,int group_id,int score,int level=0): player_id(player_id),group_id(group_id),score(score),level(level) {};
    };
    class Group {
        friend class PlayerManager;

        AVLTree levels_tree;
        int *group_zero_level_scores;
        int size;
        int scale;
    public:
        Group ()=default;
        Group(int scale) : levels_tree(false, scale), size(0), scale(scale) {
            group_zero_level_scores = new int[scale + 1];
            for (int i = 0; i <= scale; i++) {
                group_zero_level_scores[i] = 0;
            }
        }
    };

    PlayerManager(int k,int scale);
    void mergeGroups(int first_group_id,int second_group_id);
    void addPlayer(int player_id,int group_id,int score);
    void removePlayer(int player_id);
    void increasePlayerIDLevel(int player_id,int level_increase);
    void changePlayerIDScore(int player_id, int new_score);
    ~PlayerManager();

private:
    const int initial_size=5;
    AVLTree all_players_level_tree;
    HashTable<Player*> players_table;
    Group* groups_array;
    Union groups_ids;
    int* zero_level_scores;
    int k;
    int scale;

};
#endif //WET2_GIT_PLAYERSMANAGER_H
