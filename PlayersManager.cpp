#include "PlayersManager.h"
#include "Exceptions.h"
PlayerManager::PlayerManager(int k,int scale): all_players_level_tree(false,scale),k(k),scale(scale),players_table(initial_size),groups_ids(k) {
    if(k<=0 || scale<=0 || scale>200){
        throw InvalidInput();
    }
    zero_level_scores = new int[scale + 1];
    for (int i = 1; i <= scale; i++) {
        zero_level_scores[i] = 0;
    }
    groups_array = new Group[k + 1];
    for (int i = 1; i <= k; i++) {
        groups_array[i] = Group(scale);
    }
}
void PlayerManager::addPlayer(int player_id,int group_id,int score){
    if(group_id>k || group_id<=0 || player_id<=0 || score>scale || score<=0){
        throw InvalidInput();
    }
    Player player = Player(player_id,group_id,score);
    zero_level_scores[score]++;
    if(players_table.search(player_id)!=nullptr){
        throw AlreadyExists();
    }
    players_table.insert(player_id,player);//O(1)
   int real_group_id = groups_ids.find(group_id);
   groups_array[real_group_id].group_zero_level_scores[score]++;

}

void PlayerManager::removePlayer(int player_id){
    if(player_id<=0){
        throw InvalidInput();
    }
    Player* player=players_table.search(player_id);
    if(player==nullptr){
        throw DoesNotExist();
    }
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
}

void PlayerManager::increasePlayerIDLevel(int player_id,int level_increase){
    if(player_id<=0 || level_increase<=0){
        throw InvalidInput();
    }
    Player* player=players_table.search(player_id);
    if(player==nullptr){
        throw DoesNotExist(); //Does not exist
    }
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
    if(player_id<=0 || new_score > scale || new_score<=0){
        throw InvalidInput();
    }
    Player* player=players_table.search(player_id);
    if(player==nullptr){
        throw DoesNotExist(); //Does not exist
    }
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
    if(group_id<0 || group_id>k){
        throw InvalidInput();
    }
    if(score<=0 || score>=scale || higher_level<0){
        *players=0;
        return;
    }
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
        int index_group = groups_ids.find(group_id);
        Group group = groups_array[index_group];
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
        throw NoPlayers();
    }
     *players=(double)((sum_of_players_with_score_in_bounds)/sum_of_players_in_bounds)*100;

}
void PlayerManager::averageHighestPlayerLevelByGroup(int group_id, int m, double * avgLevel){
    if(group_id>k || group_id<0 || m<=0){
        throw InvalidInput();
    }
    int total_players_count=0;
    if(group_id==0){
        total_players_count+=all_players_level_tree.getTreePlayersCount();
        for(int i=1;i<=scale;i++){
            total_players_count+=zero_level_scores[i];
        }
        if(total_players_count<m){
            throw NoPlayers();
        }
        int tmp_sum;
        all_players_level_tree.getMaxAverage(m,&tmp_sum);
        *avgLevel=(double)tmp_sum/m;

    }else{
        int index_group = groups_ids.find(group_id);
        Group group = groups_array[index_group];
        total_players_count+=group.levels_tree.getTreePlayersCount();
        for(int i=1;i<=scale;i++){
            total_players_count+=group.group_zero_level_scores[i];
        }
        if(total_players_count<m){
            throw NoPlayers();//Failure
        }
        int tmp_sum;
        group.levels_tree.getMaxAverage(m,&tmp_sum);
        *avgLevel=(double)tmp_sum/m;
    }
}
void PlayerManager::mergeGroups(int first_group, int second_group){
    if(first_group<=0 || second_group<=0 || first_group>k || second_group>k){
        throw InvalidInput();
    }
    int first_group_index = groups_ids.find(first_group);
    int second_group_index = groups_ids.find(second_group);
    if(first_group_index==second_group_index){
        return;
    }
    AVLTree first_group_tree = groups_array[first_group_index].levels_tree;
    AVLTree second_group_tree = groups_array[second_group_index].levels_tree;
    int* first_group_zero_level = groups_array[first_group_index].group_zero_level_scores;
    int* second_group_zero_level = groups_array[second_group_index].group_zero_level_scores;
    int merged_groups_index = groups_ids.merge(first_group_index,second_group_index);
    int* merged_groups_zero_levels_scores= new int[scale+1];
    for(int i=1;i<=scale;i++){
        merged_groups_zero_levels_scores[i]=first_group_zero_level[i]+second_group_zero_level[i];
    }

    first_group_tree.mergeWith(second_group_tree);
    Group group = groups_array[merged_groups_index];
    group.levels_tree = first_group_tree;
    first_group_tree.deleteTree();
    second_group_tree.deleteTree();
    group.group_zero_level_scores=merged_groups_zero_levels_scores;
}
PlayerManager::~PlayerManager() {
    for(int i=1;i<=k;i++){
        groups_array[i].levels_tree.deleteTree();
        delete [] groups_array[i].group_zero_level_scores;
    }
    delete [] groups_array;
    all_players_level_tree.deleteTree();
    delete [] zero_level_scores;
}


