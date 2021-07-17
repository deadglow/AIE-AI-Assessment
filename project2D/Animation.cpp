#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
    frames.clear();
}

aie::Texture* Animation::GetFrame(int index)
{
    return frames[index];
}

void Animation::AddFrame(aie::Texture* tex)
{
    frames.push_back(tex);
}

int Animation::GetFrameCount()
{
    return frames.size();
}

std::string Animation::GetName()
{
    return name;
}

void Animation::SetName(std::string newName)
{
    name = newName;
}
