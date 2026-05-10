#include "UIEngine/composition/OldFrame.h"




OldFrame::OldFrame(BorderType type, Color fg, Color bg, int thickness) :
	type(type), bgColor(bg), fgColor(fg), thickness(thickness) {}


std::string OldFrame::GetBorder(BorderType type, BorderPlace place) {
	switch (type)
	{
	case BorderType::Simple:
		switch (place)
		{
		case BorderPlace::LineH:	return "\u2500";
		case BorderPlace::LineV:	return "\u2502";
		case BorderPlace::CornerLU:	return "\u250C";
		case BorderPlace::CornerRU:	return "\u2510";
		case BorderPlace::CornerLD:	return "\u2514";
		case BorderPlace::CornerRD:	return "\u2518";
		case BorderPlace::Cross:	return "\u253C";
		case BorderPlace::CrossL:	return "\u2524";
		case BorderPlace::CrossR:	return "\u251C";
		case BorderPlace::CrossU:	return "\u2534";
		case BorderPlace::CrossD:	return "\u252C";
		default:	return "\u253C";
		}
	case BorderType::Heavy:
		switch (place)
		{
		case BorderPlace::LineH:	return "\u2501";
		case BorderPlace::LineV:	return "\u2503";
		case BorderPlace::CornerLU:	return "\u250F";
		case BorderPlace::CornerRU:	return "\u2513";
		case BorderPlace::CornerLD:	return "\u2517";
		case BorderPlace::CornerRD:	return "\u251B";
		case BorderPlace::Cross:	return "\u254B";
		case BorderPlace::CrossL:	return "\u252B";
		case BorderPlace::CrossR:	return "\u2523";
		case BorderPlace::CrossU:	return "\u253B";
		case BorderPlace::CrossD:	return "\u2533";
		default:	return "\u254B";
		}
	case BorderType::Double:
		switch (place)
		{
		case BorderPlace::LineH:	return "\u2550";
		case BorderPlace::LineV:	return "\u2551";
		case BorderPlace::CornerLU:	return "\u2554";
		case BorderPlace::CornerRU:	return "\u2557";
		case BorderPlace::CornerLD:	return "\u255A";
		case BorderPlace::CornerRD:	return "\u255D";
		case BorderPlace::Cross:	return "\u256C";
		case BorderPlace::CrossL:	return "\u2563";
		case BorderPlace::CrossR:	return "\u2560";
		case BorderPlace::CrossU:	return "\u2569";
		case BorderPlace::CrossD:	return "\u2566";
		default:	return "\u256C";
		}
	case BorderType::Curcle:
		switch (place)
		{
		case BorderPlace::LineH:	return "\u2500";
		case BorderPlace::LineV:	return "\u2502";
		case BorderPlace::CornerLU:	return "\u256D";
		case BorderPlace::CornerRU:	return "\u256E";
		case BorderPlace::CornerLD:	return "\u2570";
		case BorderPlace::CornerRD:	return "\u256F";
		case BorderPlace::Cross:	return "\u253C";
		case BorderPlace::CrossL:	return "\u2524";
		case BorderPlace::CrossR:	return "\u251C";
		case BorderPlace::CrossU:	return "\u2534";
		case BorderPlace::CrossD:	return "\u252C";
		default:	return "\u253C";
		}
	case BorderType::ASCII:
		switch (place)
		{
		case BorderPlace::LineH:	return "-";
		case BorderPlace::LineV:	return "|";
		case BorderPlace::CornerLU:	return "+";
		case BorderPlace::CornerRU:	return "+";
		case BorderPlace::CornerLD:	return "+";
		case BorderPlace::CornerRD:	return "+";
		case BorderPlace::Cross:	return "+";
		case BorderPlace::CrossL:	return "+";
		case BorderPlace::CrossR:	return "+";
		case BorderPlace::CrossU:	return "+";
		case BorderPlace::CrossD:	return "+";
		default:	return "#";
		}
	default:
		switch (place)
		{
		case BorderPlace::LineH:	return "\u2500";
		case BorderPlace::LineV:	return "\u2502";
		case BorderPlace::CornerLU:	return "\u250C";
		case BorderPlace::CornerRU:	return "\u2510";
		case BorderPlace::CornerLD:	return "\u2514";
		case BorderPlace::CornerRD:	return "\u2518";
		case BorderPlace::Cross:	return "\u253C";
		case BorderPlace::CrossL:	return "\u2524";
		case BorderPlace::CrossR:	return "\u251C";
		case BorderPlace::CrossU:	return "\u2534";
		case BorderPlace::CrossD:	return "\u252C";
		default:	return "\u253C";
		}
	}
}

std::string OldFrame::ChangeBorder(int i, int j, BorderPlace corner, BorderPlace edgeH, BorderPlace edgeV) {
	if (i == 0 && j == 0)
		return GetBorder(corner);
	else if (i == 0)
		return GetBorder(edgeH);
	else if (j == 0)
		return GetBorder(edgeV);
	else
		return GetBorder(BorderPlace::Cross);
}

std::vector<std::vector<Pixel>> const OldFrame::PasteInFrame(std::vector<std::vector<Pixel>> picToPaste) {
	int picWidth, picHeight;
	picHeight = picToPaste.size();
	picWidth = 0;

	for (int picILenght = 0,i = 0; i < picHeight; i++)
	{
		picILenght = picToPaste[i].size();
		picWidth = picILenght > picWidth ? picILenght : picWidth;
	}



	for (int i = 0; i < thickness; i++) {
		
		picToPaste.insert(picToPaste.begin() + i, std::vector<Pixel>());

		for (int j = 0; j < thickness; j++) {
			picToPaste[i].push_back(Pixel(ChangeBorder(i,j, BorderPlace::CornerLU, BorderPlace::CrossD, BorderPlace::CrossR), fgColor, bgColor));
		}

		picToPaste[i].insert(picToPaste[i].end(), picWidth, Pixel(GetBorder(BorderPlace::LineH), fgColor, bgColor));

		for (int j = 0; j < thickness; j++) {
			picToPaste[i].push_back(Pixel(ChangeBorder(i, thickness - j - 1, BorderPlace::CornerRU, BorderPlace::CrossD, BorderPlace::CrossL), fgColor, bgColor));
		}
	}

	for (int i = thickness; i < thickness + picHeight; i++) {
		picToPaste[i].insert(picToPaste[i].end(), picWidth - picToPaste[i].size(), Pixel(" ", fgColor, bgColor));
		picToPaste[i].insert(picToPaste[i].begin(), thickness, Pixel(GetBorder(BorderPlace::LineV), fgColor, bgColor));
		picToPaste[i].insert(picToPaste[i].end(), thickness, Pixel(GetBorder(BorderPlace::LineV), fgColor, bgColor));
	}

	for (int i = 0; i < thickness; i++) {

		picToPaste.insert(picToPaste.begin() + picHeight + thickness + i, std::vector<Pixel>());
		for (int j = 0; j < thickness; j++) {
			picToPaste[picHeight + thickness + i].push_back(Pixel(ChangeBorder(thickness - i - 1, j, BorderPlace::CornerLD, BorderPlace::CrossU, BorderPlace::CrossR), fgColor, bgColor));
		}

		picToPaste[picHeight + thickness + i].insert(picToPaste[picHeight + thickness + i].end(), picWidth, Pixel(GetBorder(BorderPlace::LineH), fgColor, bgColor));

		for (int j = 0; j < thickness; j++) {
			picToPaste[picHeight + thickness + i].push_back(Pixel(ChangeBorder(thickness - i - 1, thickness - j - 1, BorderPlace::CornerRD, BorderPlace::CrossU, BorderPlace::CrossL), fgColor, bgColor));
		}
	}
	return picToPaste;
}
