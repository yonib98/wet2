#include "PlayersManager.h"
PlayerManager::PlayerManager(int k,int scale): all_players_level_tree(false,scale),k(k),scale(scale),players_table(initial_size),groups_ids(k){
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

void PlayerManager::removePlayer(int player_id){
    Player* player=players_table.search(player_id);
    int player_level=player->level;
    int player_score=player->score;
    int player_group_id=player->group_id;
    int index_group= groups_ids.find(player_group_id);
    Group group=groups_array[index_group];
    if(player_level==0){
        zero_level_scores[player_score]--;
        group.group_zero_level_scores[player_score]--;
    }
    else
    {
        all_players_level_tree.remove(player_level,0,player_score);
        group.levels_tree.remove(player_level,0,player_score);
    }
    players_table.remove(player_id);
    delete player;
}

void PlayerManager::increasePlayerIDLevel(int player_id,int level_increase){
    Player* player=players_table.search(player_id);
    int old_level=player->level;
    int player_group_id=player->group_id;
    int index_group= groups_ids.find(player_group_id);
    Group group=groups_array[index_group];
    player->level+=level_increase;
    if(old_level==0){
        zero_level_scores[player->score]--;
        all_players_level_tree.insert(player->level,0,player->score);
        group.group_zero_level_scores[scale]--;
        group.levels_tree.insert(player->level,0,player->score);
    }
    else
    {
        all_players_level_tree.remove(old_level,0,player->score);
        all_players_level_tree.insert(player->level,0,player->score);
        group.levels_tree.remove(old_level,0,player->score);
        group.levels_tree.insert(player->level,0,player->score);
    }
}

void PlayerManager::changePlayerIDScore(int player_id, int new_score){
    Player* player=players_table.search(player_id);
    int player_level=player->level;
    int old_score=player->score;
    player->score=new_score;
    int player_group_id=player->group_id;
    int index_group= groups_ids.find(player_group_id);
    Group group=groups_array[index_group];
    if(player_level==0){
        zero_level_scores[old_score]--;
        zero_level_scores[new_score]++;
        group.group_zero_level_scores[old_score]--;
        group.group_zero_level_scores[new_score]++;
    }
    else{
        all_players_level_tree.remove(player_level,0,old_score);
        all_players_level_tree.insert(player_level,0,new_score);
        group.levels_tree.remove(player_level,0,old_score);
        group.levels_tree.insert(player_level,0,new_score);
    }
}
void PlayerManager::getPercentOfPlayersWithScoreInBounds (int group_id, int score, int lower_level, int higher_level, double * players){
    int sum_of_players_in_bounds=0;
    int sum_of_players_with_score_in_bounds=0;
    if(group_id==0) {
        all_players_level_tree.scoresInBounds(lower_level, higher_level, score, &sum_of_players_in_bounds,
                                              &sum_of_players_with_score_in_bounds);
        if(lower_level<=0){
            for(int i=1;i<=scale;i++){
                sum_of_players_in_bounds+=zero_level_scores[i];
            }
            sum_of_players_with_score_in_bounds+=zero_level_scores[score];
        }
    }
    else{
        group.levels_tree.scoresInBounds(lower_level, higher_level, score, &sum_of_players_in_bounds,
                                        &sum_of_players_with_score_in_bounds);
        if(lower_level<=0){
            for(int i=1;i<=scale;i++){
                sum_of_players_in_bounds+=group.group_zero_level_scores[i];
            }
            sum_of_players_with_score_in_bounds+=group.group_zero_level_scores[score];
        }

        }
    if(sum_of_players_with_score_in_bounds==0){
        throw int();//Failure
    }
     *players=(double)(sum_of_players_with_score_in_bounds/sum_of_players_in_bounds)*100;

}
void PlayerManager::averageHighestPlayerLevelByGroup(int group_id, int m, double * avgLevel){
    int total_players_count=0;
    if(group_id==0){
        total_players_count+=all_players_level_tree.root->getSubTreePlayersCount();
        for(int i=1;i<=scale;i++){
            total_players_count+=zero_level_scores[i];
        }
        if(total_players_count<m){
            throw int();//Failure
        }
        int tmp_sum;
        all_players_level_tree.getMaxAverage(m,&tmp_sum);
        *avgLevel=(double)tmp_sum/m;

    }else{
        int index_group = groups_ids.find(group_id);
        Group group = groups_array[index_group];
        total_players_count+=group.levels_tree.root->getSubTreePlayersCount();
        for(int i=1;i<=scale;i++){
            total_players_count+=group.group_zero_level_scores[i];
        }
        if(total_players_count<m){
            throw int();//Failure
        }
        int tmp_sum;
        group.levels_tree.getMaxAverage(m,&tmp_sum);
        *avgLevel=(double)tmp_sum/m;
    }
}

