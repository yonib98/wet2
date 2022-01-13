#include "PlayersManager.h"
#include "Exceptions.h"
PlayerManager::PlayerManager(int k,int scale): all_players_level_tree(false,scale),players_table(initial_size),groups_ids(k),k(k),scale(scale) {
    if(k<=0 || scale<=0 || scale>200){
        throw InvalidInput();
    }
    zero_level_scores = new int[scale + 1];
    for (int i = 1; i <= scale; i++) {
        zero_level_scores[i] = 0;
    }
    groups_array = new Group*[k + 1];
    for (int i = 1; i <= k; i++) {
        groups_array[i] = new Group(scale);
    }
}
void PlayerManager::addPlayer(int player_id,int group_id,int score){
    static int counter=0;
    counter++;
    if(group_id>k || group_id<=0 || player_id<=0 || score>scale || score<=0){
        throw InvalidInput();
    }
    Player player = Player(player_id,group_id,score);
    if(players_table.search(player_id)!=nullptr){
        throw AlreadyExists();
    }
    zero_level_scores[score]++;
    players_table.insert(player_id,player);//O(1)
   int real_group_id = groups_ids.find(group_id);
   groups_array[real_group_id]->group_zero_level_scores[score]++;

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
    Group* group=groups_array[index_group];
    if(player_level==0){
        zero_level_scores[player_score]--;
        group->group_zero_level_scores[player_score]--;
    }
    else
    {
        all_players_level_tree.remove(player_level,0,player_score);
        group->levels_tree.remove(player_level,0,player_score);
    }
    players_table.remove(player_id);
}

void PlayerManager::increasePlayerIDLevel(int player_id,int level_increase){
    if(player_id==1271352989 ){
        int x=1;
    }
    if(player_id<=0 || level_increase<=0){
        throw InvalidInput();
    }
    Player* player=players_table.search(player_id);
    int count=all_players_level_tree.getTreePlayersCount();
    if(player==nullptr){
        throw DoesNotExist(); //Does not exist
    }
    int old_level=player->level;
    int player_group_id=player->group_id;
    int index_group= groups_ids.find(player_group_id);
    Group* group=groups_array[index_group];
    player->level+=level_increase;
    if(old_level==0){
        zero_level_scores[player->score]--;
        all_players_level_tree.insert(player->level,0,player->score);
        group->group_zero_level_scores[player->score]--;
        group->levels_tree.insert(player->level,0,player->score);
    }
    else
    {
        all_players_level_tree.remove(old_level,0,player->score);
        all_players_level_tree.insert(player->level,0,player->score);
        group->levels_tree.remove(old_level,0,player->score);
        group->levels_tree.insert(player->level,0,player->score);
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
    Group* group=groups_array[index_group];
    if(player_level==0){
        zero_level_scores[old_score]--;
        zero_level_scores[new_score]++;
        group->group_zero_level_scores[old_score]--;
        group->group_zero_level_scores[new_score]++;
    }
    else{
        all_players_level_tree.remove(player_level,0,old_score);
        all_players_level_tree.insert(player_level,0,new_score);
        group->levels_tree.remove(player_level,0,old_score);
        group->levels_tree.insert(player_level,0,new_score);
    }
}
void PlayerManager::getPercentOfPlayersWithScoreInBounds (int group_id, int score, int lower_level, int higher_level, double * players) {
    if (group_id < 0 || group_id > k) {
        throw InvalidInput();
    }
    if(lower_level>higher_level){
        throw NoPlayers();
    }
    int sum_of_players_in_bounds = 0;
    int sum_of_players_with_score_in_bounds = 0;
    if (group_id == 0) {
        all_players_level_tree.scoresInBounds(lower_level, higher_level, score, &sum_of_players_in_bounds,
                                              &sum_of_players_with_score_in_bounds);
        if (lower_level <= 0) {
            for (int i = 1; i <= scale; i++) {
                sum_of_players_in_bounds += zero_level_scores[i];
            }
            sum_of_players_with_score_in_bounds += zero_level_scores[score];
        }
    } else {
        int index_group = groups_ids.find(group_id);
        Group *group = groups_array[index_group];
        group->levels_tree.scoresInBounds(lower_level, higher_level, score, &sum_of_players_in_bounds,
                                          &sum_of_players_with_score_in_bounds);
        if (lower_level <= 0) {
            for (int i = 1; i <= scale; i++) {
                sum_of_players_in_bounds += group->group_zero_level_scores[i];
            }
            sum_of_players_with_score_in_bounds += group->group_zero_level_scores[score];
        }

    }
    if (sum_of_players_in_bounds == 0) {
        throw NoPlayers();
    }
    *players = (((double) (sum_of_players_with_score_in_bounds)) / sum_of_players_in_bounds) * 100;
}

void PlayerManager::averageHighestPlayerLevelByGroup(int group_id, int m, double * avgLevel){
    if(group_id==0 && m==21){
        int x=1;
    }
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
        Group* group = groups_array[index_group];
        total_players_count+=group->levels_tree.getTreePlayersCount();
        for(int i=1;i<=scale;i++){
            total_players_count+=group->group_zero_level_scores[i];
        }
        if(total_players_count<m){
            throw NoPlayers();//Failure
        }
        int tmp_sum;
        group->levels_tree.getMaxAverage(m,&tmp_sum);
        *avgLevel=(double)tmp_sum/m;
    }
}
void PlayerManager::mergeGroups(int first_group, int second_group){
    static int counter=0;
    counter++;
    if(first_group<=0 || second_group<=0 || first_group>k || second_group>k){
        throw InvalidInput();
    }
    int first_group_index = groups_ids.find(first_group);
    int second_group_index = groups_ids.find(second_group);
    if(first_group_index==second_group_index){
        return;
    }

    int* first_group_zero_level = groups_array[first_group_index]->group_zero_level_scores;
    int* second_group_zero_level = groups_array[second_group_index]->group_zero_level_scores;
    int merged_groups_index = groups_ids.merge(first_group_index,second_group_index);
    int* merged_groups_zero_levels_scores= new int[scale+1];
    for(int i=1;i<=scale;i++){
        merged_groups_zero_levels_scores[i]=first_group_zero_level[i]+second_group_zero_level[i];
    }

    Group* group = groups_array[merged_groups_index];
    if(merged_groups_index==first_group_index){
        groups_array[first_group_index]->levels_tree.mergeWith(groups_array[second_group_index]->levels_tree);
        groups_array[second_group_index]->levels_tree.deleteTree();
    }
    else{
        groups_array[second_group_index]->levels_tree.mergeWith(groups_array[first_group_index]->levels_tree);
        groups_array[first_group_index]->levels_tree.deleteTree();
    }
    delete [] group->group_zero_level_scores;
    group->group_zero_level_scores=merged_groups_zero_levels_scores;
}
void PlayerManager::GetPlayersBound(int group_id, int score, int m,
                     int * LowerBoundPlayers, int * HigherBoundPlayers){
    if(group_id==0 && score==1 && m==5){
        int x=1;
    }
    if(group_id<0 || group_id > k || m<0 || score<=0 || score>scale){
        throw InvalidInput();
    }
    *LowerBoundPlayers=0;
    *HigherBoundPlayers=0;
    if(group_id==0){
        int players_without_level_0=all_players_level_tree.getTreePlayersCount();
        int zero_level_count=0;
        for(int i=1;i<=scale;i++){
            zero_level_count+=zero_level_scores[i];
        }
        if(players_without_level_0+zero_level_count<m){
            throw NoPlayers();
        }
        if(players_without_level_0>=m){
            all_players_level_tree.getScoresBounds(score,m,LowerBoundPlayers,HigherBoundPlayers);
        }else {
            all_players_level_tree.getScoresBounds(score, players_without_level_0, LowerBoundPlayers,
                                                   HigherBoundPlayers);
            int dup_m=m;
            m-=players_without_level_0;
            if(zero_level_scores[score]>=m) {
                *HigherBoundPlayers += m;
            }
            else{
                *HigherBoundPlayers += zero_level_scores[score];
            }
            int num_of_players_to_add = dup_m - players_without_level_0;
            int num_of_players_without_the_score = zero_level_count-zero_level_scores[score];
            if (num_of_players_without_the_score >= num_of_players_to_add) {
                *LowerBoundPlayers += 0;
            }
            else{
                *LowerBoundPlayers += num_of_players_to_add - num_of_players_without_the_score;
            }
        }
    }else{
        int group_index=groups_ids.find(group_id);
        Group* group=groups_array[group_index];
        int players_without_level_0=group->levels_tree.getTreePlayersCount();
        int zero_level_count=0;
        for(int i=1;i<=scale;i++){
            zero_level_count+=group->group_zero_level_scores[i];
        }
        if(players_without_level_0+zero_level_count<m){
            throw NoPlayers();
        }
        if(players_without_level_0>=m){
            group->levels_tree.getScoresBounds(score,m,LowerBoundPlayers,HigherBoundPlayers);
        }else{
            group->levels_tree.getScoresBounds(score,players_without_level_0,LowerBoundPlayers,HigherBoundPlayers);
            int dup_m=m;
            m-=players_without_level_0;
            if(group->group_zero_level_scores[score]>=m) {
                *HigherBoundPlayers += m;
            }
            else{
                *HigherBoundPlayers+=group->group_zero_level_scores[score];
            }
                int num_of_players_to_add = dup_m-players_without_level_0;
            int num_of_players_without_the_score=zero_level_count-group->group_zero_level_scores[score];
            if (num_of_players_without_the_score >= num_of_players_to_add) {
                *LowerBoundPlayers += 0;
            }
            else{
                *LowerBoundPlayers += num_of_players_to_add - num_of_players_without_the_score;
            }
        }
    }
}
PlayerManager::~PlayerManager(){
    for(int i=1;i<=k;i++){
        groups_array[i]->levels_tree.deleteTree();
        delete groups_array[i];
    }
    delete [] groups_array;
    all_players_level_tree.deleteTree();
    delete [] zero_level_scores;
}


