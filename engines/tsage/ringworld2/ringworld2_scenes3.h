/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef TSAGE_RINGWORLD2_SCENES3_H
#define TSAGE_RINGWORLD2_SCENES3_H

#include "common/scummsys.h"
#include "tsage/converse.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"
#include "tsage/sound.h"
#include "tsage/ringworld2/ringworld2_logic.h"
#include "tsage/ringworld2/ringworld2_speakers.h"

namespace TsAGE {

namespace Ringworld2 {

using namespace TsAGE;


class Scene3100 : public SceneExt {
	class Actor6 : public SceneActor {
		virtual bool startAction(CursorType action, Event &event);
	};
public:

	int _field412;
	SpeakerGuard3100 _guardSpeaker;
	NamedHotspot _item1;
	NamedHotspot _item2;
	SceneActor _actor1;
	SceneActor _actor2;
	SceneActor _actor3;
	SceneActor _actor4;
	SceneActor _actor5;
	Actor6 _actor6;
	ASoundExt _sound1;
	SequenceManager _sequenceManager;

	Scene3100();
	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void signal();
	virtual void dispatch();
	virtual void synchronize(Serializer &s);
};

class Scene3125 : public SceneExt {
	class Item1 : public NamedHotspot {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};
	class Item2 : public Item1 {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};
	class Item3 : public Item1 {
	public:
		virtual bool startAction(CursorType action, Event &event);
	};

	class Actor1 : public SceneActor {
		virtual bool startAction(CursorType action, Event &event);
	};
public:

	int _field412;
	Item1 _item1;
	Actor1 _actor1;
	Item2 _item2;
	Item3 _item3;
	SceneActor _actor2;
	SceneActor _actor3;
	SceneActor _actor4;
	SceneActor _actor5;
	SequenceManager _sequenceManager1;
	// Second sequence manager... Unused?
	SequenceManager _sequenceManager2;

	Scene3125();
	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void signal();
	virtual void dispatch();
	virtual void synchronize(Serializer &s);
};
} // End of namespace Ringworld2
} // End of namespace TsAGE

#endif
