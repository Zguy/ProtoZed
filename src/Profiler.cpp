/*
Copyright (c) 2012 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <ProtoZed/Profiler.h>

#include <ProtoZed/Clock.h>
#include <ProtoZed/Convert.h>

#include <vector>
#include <fstream>
#include <limits>

namespace PZ
{
	struct ProfileBlock
	{
		ProfileBlock(const std::string &name) : name(name), totalTime(0.f), bestTime(std::numeric_limits<float>::max()), worstTime(0.f), calls(0), bestCall(0), worstCall(0), parent(nullptr), nextSibling(nullptr), firstChild(nullptr), lastChild(nullptr)
		{}

		void AddTime(float time)
		{
			totalTime += time;

			if (time < bestTime)
			{
				bestTime = time;
				bestCall = calls;
			}
			if (time > worstTime)
			{
				worstTime = time;
				worstCall = calls;
			}
		}

		float GetAverage() const
		{
			return (totalTime / static_cast<float>(calls));
		}

		std::string name;
		float beginTime;

		float totalTime;
		float bestTime;
		float worstTime;

		int calls;
		int bestCall;
		int worstCall;

		ProfileBlock *parent;
		ProfileBlock *nextSibling;
		ProfileBlock *firstChild;
		ProfileBlock *lastChild;
	};

	class Profiler::Impl
	{
	public:
		Impl() : rootBlock(nullptr), currentBlock(nullptr)
		{}

		ProfileBlock *getNewBlock(ProfileBlock *parent, const std::string &name)
		{
			ProfileBlock *block = new ProfileBlock(name);

			if (parent != nullptr)
			{
				if (parent->lastChild == nullptr)
					parent->firstChild = block;
				else
					parent->lastChild->nextSibling = block;
				parent->lastChild = block;
				block->parent = parent;
			}

			return block;
		}
		ProfileBlock *getBlockByName(ProfileBlock *parent, const std::string &name)
		{
			if (parent != nullptr)
			{
				for (ProfileBlock *current = parent->firstChild; current != nullptr; current = current->nextSibling)
				{
					if (current->name == name)
						return current;
				}
			}

			return nullptr;
		}
		void getAllBlocks(ProfileBlock *parent, std::vector<ProfileBlock*> &list)
		{
			list.clear();

			for (ProfileBlock *current = parent; current != nullptr;)
			{
				list.push_back(current);

				if (current->firstChild != nullptr)
				{
					current = current->firstChild;
				}
				else if (current->nextSibling != nullptr)
				{
					current = current->nextSibling;
				}
				else
				{
					ProfileBlock *currParent = current->parent;
					current = nullptr;
					while (current == nullptr && currParent != nullptr)
					{
						current = currParent->nextSibling;
						currParent = currParent->parent;
					}
				}
			}
		}

		Clock frameClock;

		float targetFrameTime;

		ProfileBlock *rootBlock;
		ProfileBlock *currentBlock;

		std::vector<ProfileBlock*> frames;
	};

	Profiler::Profiler() : p(new Impl)
	{
	}
	Profiler::~Profiler()
	{
		// Clear blocks
		std::vector<ProfileBlock*> allBlocks;
		for (std::vector<ProfileBlock*>::const_iterator it = p->frames.begin(); it != p->frames.end(); ++it)
		{
			p->getAllBlocks((*it), allBlocks);
			for (std::vector<ProfileBlock*>::iterator it = allBlocks.begin(); it != allBlocks.end(); ++it)
			{
				delete (*it);
			}
			allBlocks.clear();
		}
		p->frames.clear();

		delete p;
	}

#ifdef PROFILER
	void Profiler::Begin(const std::string &name)
	{
		ProfileBlock *block = p->getBlockByName(p->currentBlock, name);
		if (block == nullptr)
			block = p->getNewBlock(p->currentBlock, name);

		block->beginTime = p->frameClock.GetElapsedTime();
		block->calls++;

		p->currentBlock = block;
	}
	void Profiler::End()
	{
		ProfileBlock *block = p->currentBlock;

		assert(block != p->rootBlock); // Assert that we aren't removing the last block (this probably means an End() call without a Begin() call)

		float endTime  = p->frameClock.GetElapsedTime();
		float callTime = endTime - block->beginTime;
		block->AddTime(callTime);

		p->currentBlock = block->parent;
	}

	void Profiler::Start()
	{
		if (p->rootBlock != nullptr)
			return;

		p->rootBlock = new ProfileBlock("Frame #"+Convert::ToString(p->frames.size()));

		p->rootBlock->beginTime = p->frameClock.GetElapsedTime();
		p->rootBlock->calls++;

		p->currentBlock = p->rootBlock;
	}
	void Profiler::Stop()
	{
		float endTime  = p->frameClock.GetElapsedTime();
		float callTime = endTime - p->rootBlock->beginTime;
		p->rootBlock->AddTime(callTime);

		p->frames.push_back(p->rootBlock);
		p->rootBlock = nullptr;
		p->currentBlock = nullptr;
	}

	void Profiler::NextFrame()
	{
		assert(p->currentBlock == p->rootBlock); // Assert that there aren't any currently active blocks when we're switching to the next frame (this probably means a Begin() call without an End() call)

		Stop();
		Start();
	}

	void Profiler::SetTargetFrameTime(float target)
	{
		p->targetFrameTime = target;
	}

	void Profiler::WriteLog(const std::string &filename) const
	{
		//TODO: Maybe output as json instead?
		std::fstream log(filename+".log", std::ios::out | std::ios::trunc);

		ProfileBlock *root;
		for (std::vector<ProfileBlock*>::const_iterator it = p->frames.begin(); it != p->frames.end(); ++it)
		{
			root = (*it);

			log << root->name << "\n";

			std::string path;
			for (ProfileBlock *current = root; current != nullptr;)
			{
				path += current->name;

				float avgOfFrame = (current->GetAverage() / root->GetAverage()) * 100.f;

				log.precision(3);
				log << "avg: " << current->GetAverage() * 1000.f << " ms";
				log << " (" << avgOfFrame << "%)\t";
				log << "best: " << current->bestTime * 1000.f << " ms";
				log << " [" << current->bestCall << "]\t";
				log << "worst: " << current->worstTime * 1000.f << " ms";
				log << " [" << current->worstCall << "]\t";
				log << "calls: " << current->calls << "\t";
				log << path << "\n";

				if (current->firstChild != nullptr)
				{
					current = current->firstChild;

					path += '/';
				}
				else if (current->nextSibling != nullptr)
				{
					current = current->nextSibling;

					path = path.substr(0, path.rfind('/')+1);
				}
				else
				{
					path = path.substr(0, path.rfind('/')+1);

					ProfileBlock *currParent = current->parent;
					current = nullptr;
					while (current == nullptr && currParent != nullptr)
					{
						current = currParent->nextSibling;
						currParent = currParent->parent;

						path = path.substr(0, path.rfind('/')+1);
					}
				}
			}

			log << "\n";
		}

		log.close();
	}
#endif

	Profile::Profile(const std::string &name) : profiler(Profiler::GetSingleton())
	{
		profiler.Begin(name);
	}
	Profile::~Profile()
	{
		profiler.End();
	}
}