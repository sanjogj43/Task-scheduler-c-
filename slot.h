#ifndef SLOT_H
#define SLOT_H

class resource;

class slot
{
	public:
		int start;
		int end;
		int slotDuration;
		resource *resourceOnSlot;
		slot(int start, int end)
		{
			this->start = start;
			this->end = end;
			this->slotDuration = end - start+1;
			this->resourceOnSlot = NULL;
		}
};

#endif
