
struct VolumeCmpare
{
	bool operator()(double newValue,double oldValue){return int(newValue * 100) == int(oldValue * 100); };
};