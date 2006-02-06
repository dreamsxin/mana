/*
 *  The Mana World
 *  Copyright 2004 The Mana World Development Team
 *
 *  This file is part of The Mana World.
 *
 *  The Mana World is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  any later version.
 *
 *  The Mana World is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The Mana World; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  $Id$
 */

#include "joystick.h"

#include "configuration.h"
#include "log.h"

int Joystick::joystickCount = 0;

void Joystick::init()
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    //SDL_JoystickEventState(SDL_ENABLE);
    joystickCount = SDL_NumJoysticks();
    logger->log("%i joysticks/gamepads found", joystickCount);
    for (int i = 0; i < joystickCount; i++)
        logger->log("- %s", SDL_JoystickName(i));
}

Joystick::Joystick(int no):
    mDirection(0), mCalibrating(false)
{
    // TODO Bail out here?
    if (no > joystickCount)
        return;

    mJoystick = SDL_JoystickOpen(no);

    // TODO Bail out!
    if (!mJoystick)
    {
        logger->log("Couldn't open joystick: %s", SDL_GetError());
        return;
    }

    logger->log("Axes: %i ", SDL_JoystickNumAxes(mJoystick));
    logger->log("Balls: %i", SDL_JoystickNumBalls(mJoystick));
    logger->log("Hats: %i", SDL_JoystickNumHats(mJoystick));
    logger->log("Buttons: %i", SDL_JoystickNumButtons(mJoystick));

    mUpTolerance = (int)config.getValue("upTolerance", 100);
    mDownTolerance = (int)config.getValue("downTolerance", 100);
    mLeftTolerance = (int)config.getValue("leftTolerance", 100);
    mRightTolerance = (int)config.getValue("rightTolerance", 100);
}

Joystick::~Joystick()
{
        SDL_JoystickClose(mJoystick);
}

void Joystick::update()
{
    mDirection = 0;
    SDL_JoystickUpdate();

    // When calibrating, don't bother the outside with our state
    if (mCalibrating) {
        doCalibration();
        return;
    };

    // X-Axis
    int position = SDL_JoystickGetAxis(mJoystick, 0);
    if (position >= mRightTolerance)
    {
        mDirection |= RIGHT;
    }
    else if (position <= mLeftTolerance)
    {
        mDirection |= LEFT;
    }

    // Y-Axis
    position = SDL_JoystickGetAxis(mJoystick, 1);
    if (position <= mUpTolerance)
    {
        mDirection |= UP;
    }
    else if (position >= mDownTolerance)
    {
        mDirection |= DOWN;
    }

    // Buttons
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        mButtons[i] = (SDL_JoystickGetButton(mJoystick, i) == 1);
    }
}

void Joystick::startCalibration()
{
    mUpTolerance = 0;
    mDownTolerance = 0;
    mLeftTolerance = 0;
    mRightTolerance = 0;
    mCalibrating = true;
}

void Joystick::doCalibration()
{
    // X-Axis
    int position = SDL_JoystickGetAxis(mJoystick, 0);
    if (position > mRightTolerance)
    {
        mRightTolerance = position;
    }
    else if (position < mLeftTolerance)
    {
        mLeftTolerance = position;
    }

    // Y-Axis
    position = SDL_JoystickGetAxis(mJoystick, 1);
    if (position > mDownTolerance)
    {
        mDownTolerance = position;
    }
    else if (position < mUpTolerance)
    {
        mUpTolerance = position;
    }
}


void Joystick::finishCalibration()
{
    config.setValue("leftTolerance", mLeftTolerance);
    config.setValue("rightTolerance", mRightTolerance);
    config.setValue("upTolerance", mUpTolerance);
    config.setValue("downTolerance", mDownTolerance);
    mCalibrating = false;
}

bool Joystick::buttonPressed(unsigned char no)
{
    if (no > MAX_BUTTONS)
        return false;

    return mButtons[no];
}