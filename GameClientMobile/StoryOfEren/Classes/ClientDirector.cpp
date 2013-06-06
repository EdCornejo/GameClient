//
//  ClientDirector.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/25/13.
//
//

#include "Headers.pch"

bool ClientDirector::IsRenderingWorkerRoutinePaused()
{
    return (GameClient::Instance().GetRenderingTaskWorkerRoutine().IsPaused() == true);
}

void ClientDirector::PauseRenderingTaskWorkerRoutine()
{
    GameClient::Instance().GetRenderingTaskWorkerRoutine().Pause();
}

void ClientDirector::ResumeRenderingTaskWorkerRoutine()
{
    GameClient::Instance().GetRenderingTaskWorkerRoutine().Resume();
}