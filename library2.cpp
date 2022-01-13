#include "library2.h"
#include "PlayersManager.h"

//Invalid_input->AllocationError->Failure->Success
void *Init(int k, int scale){
    PlayerManager *DS;
    try{
        DS = new PlayerManager(k,scale);
    }
    catch (InvalidInput& e){
        return NULL;
    }
    catch (std::bad_alloc& e){
        return NULL;
    }
    return (void*)DS;
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm = (PlayerManager*)DS;
        pm->mergeGroups(GroupID1,GroupID2);
    }
    catch (InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm = (PlayerManager*)DS;
        pm->addPlayer(PlayerID,GroupID,score);
    }
    catch (InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch (AlreadyExists& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm = (PlayerManager*)DS;
        pm->removePlayer(PlayerID);
    }
    catch(InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm=(PlayerManager*)DS;
        pm->increasePlayerIDLevel(PlayerID,LevelIncrease);
    }
    catch(InvalidInput& e){
        return INVALID_INPUT;
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch(DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if(DS==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm=(PlayerManager*)DS;
        pm->changePlayerIDScore(PlayerID,NewScore);
    }
    catch (InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch (DoesNotExist& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                                double * players){
    if(DS==NULL || players==NULL){
        return INVALID_INPUT;
    }
    try{
        PlayerManager* pm=(PlayerManager*)DS;
        pm->getPercentOfPlayersWithScoreInBounds(GroupID,score,lowerLevel,higherLevel,players);
    }
    catch (InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (std::bad_alloc&e){
        return ALLOCATION_ERROR;
    }
    catch (NoPlayers& e){
        return FAILURE;
    }
    return SUCCESS;
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    if(DS==NULL || level==NULL){
        return INVALID_INPUT;
    }try{
        PlayerManager* pm=(PlayerManager*)DS;
        pm->averageHighestPlayerLevelByGroup(GroupID,m,level);
    }
    catch(InvalidInput& e){
        return INVALID_INPUT;
    }
    catch (std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch (NoPlayers& e){
        return FAILURE;
    }
    return SUCCESS;
}


StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                           int * LowerBoundPlayers, int * HigherBoundPlayers){
    if(DS==NULL || LowerBoundPlayers==NULL || HigherBoundPlayers==NULL){
        throw InvalidInput();
    }
    try{
        PlayerManager* pm=(PlayerManager*)DS;
        pm->GetPlayersBound(GroupID,score,m,LowerBoundPlayers,HigherBoundPlayers);
    }
    catch(InvalidInput& e){
        return INVALID_INPUT;
    }
    catch(std::bad_alloc& e){
        return ALLOCATION_ERROR;
    }
    catch (NoPlayers& e){
        return FAILURE;
    }
    return SUCCESS;
}


void Quit(void** DS){
    if(DS==NULL){
        return;
    }
    PlayerManager* pm =((PlayerManager*)*DS);
    delete pm;
    *DS=NULL;
}