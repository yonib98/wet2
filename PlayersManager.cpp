#include "PlayersManager.h"
PlayerManager::PlayerManager(int k,int scale): k(k),scale(scale),players_table(initial_size),groups_ids(k){
    zero_level_scores=new int[scale+1];
    for(int i=1;i<=scale;i++){
        zero_level_scores[i]=0;
    }
    groups_array = new Group[k+1];
    for(int i=1;i<=k;i++){
        groups_array[i]=Group(scale);
    }

}
void PlayerManager::addPlayer(int player_id,int group_id,int score){
    Player* player = new Player(player_id,group_id,score);
    zero_level_scores[score]++;
    if(players_table.search(player_id)!=nullptr){
        throw int(); //already_exist
    }
    players_table.insert(player_id,player);//O(1)
   int real_group_id = groups_ids.find(group_id);
   groups_array[real_group_id].group_zero_level_scores[score]++;

}
void removePlayer(int player_id);