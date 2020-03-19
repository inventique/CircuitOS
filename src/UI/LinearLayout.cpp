#include "LinearLayout.h"

LinearLayout::LinearLayout(ElementContainer* parent, LayoutDirection direction) : Layout(parent), direction(direction){

}

void LinearLayout::draw(){
	// Vertical
	Serial.println("Drawing linear layout");

	sprite->clear(TFT_BLACK);

	int x = padding;
	int y = padding;

	for(Element* el : children){
		el->sprite->setPos(x, y);
		el->draw();

		if(direction == VERTICAL){
			y += gutter + el->getHeight();
		}else if(direction == HORIZONTAL){
			x += gutter + el->getWidth();
		}
	}

	Element::draw();

	// TODO: overflow: draw over padding or erase?

	sprite->push();
}

void LinearLayout::reflow(){
	// TODO WHType, current height: children, width: parent

	width = wType == FIXED ? width : 0;
	height = hType == FIXED ? height : 0;

	if(direction == VERTICAL){
		reflowVertical();
	}else if(direction == HORIZONTAL){
		reflowHorizontal();
	}

	Serial.println("Reflowing linear layout [" + String(width) + ", " + String(height) + "]");
	Serial.println("W/H Type " + String(wType) + ", " + String(hType) + " [ FIXED, CHILDREN, PARENT ]");

	// call setWidth and setHeight for any potential future functionality
	setWidth(width);
	setHeight(height);

	resize(width, height);
}

void LinearLayout::reflowHorizontal(){
	if(hType == PARENT){
		height = getParent()->getHeight();
	}else if(hType == CHILDREN && !children.empty()){
		uint maxHeight = 0;

		for(const Element* el : children){
			maxHeight = max(maxHeight, el->getHeight());
		}

		height = maxHeight + 2 * padding;
	}

	if(wType == PARENT){
		width = getParent()->getAvailableWidth();
	}else if(wType == CHILDREN){
		width += 2 * padding;

		if(!children.empty()){
			for(const Element* el : children){
				width += el->getWidth() + gutter;
			}

			width -= gutter;
		}
	}
}

void LinearLayout::reflowVertical(){
	if(hType == PARENT){
		height = getParent()->getHeight();
	}else if(hType == CHILDREN){
		height += 2 * padding;

		if(!children.empty()){
			for(const Element* el : children){
				height += el->getHeight() + gutter;
			}

			height -= gutter;
		}
	}

	if(wType == PARENT){
		width = getParent()->getAvailableWidth();
	}else if(wType == CHILDREN && !children.empty()){
		uint maxWidth = 0;

		for(const Element* el : children){
			maxWidth = max(maxWidth, el->getWidth());
		}

		width = maxWidth + 2 * padding;
	}
}

uint LinearLayout::getAvailableWidth(){
	return width - 2 * padding;
}

uint LinearLayout::getAvailableHeight(){
	return height - 2 * padding;
}

uint LinearLayout::getWidth() const{
	return width;
}

uint LinearLayout::getHeight() const{
	return height;
}

