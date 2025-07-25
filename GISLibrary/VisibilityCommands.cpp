#include "stdafx.h"
#include "VisibilityCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingInstructions
{
    VisibilityCommands::~VisibilityCommands() {
        delete viewingGroup;
		delete displayPlane;
		delete drawingPriority;
		delete scaleMinimum;
		delete scaleMaximum;
		delete id;
		delete parent;
		delete hover;
    }
    
    void VisibilityCommands::setViewingGroup(const std::vector<std::string>& viewingGroups) {
        delete this->viewingGroup;
        viewingGroup = new ViewingGroup(viewingGroups);
    }

    void VisibilityCommands::setDisplayPlane(const std::string& displayPlane) {
        delete this->displayPlane;
        this->displayPlane = new DisplayPlane(displayPlane);
    }

    void VisibilityCommands::setDrawingPriority(int drawingPriority) {
        delete this->drawingPriority;
        this->drawingPriority = new DrawingPriority(drawingPriority);
    }

    void VisibilityCommands::setScaleMinimum(int scaleMinimum) {
        delete this->scaleMinimum;
        this->scaleMinimum = new ScaleMinimum(scaleMinimum);
    }

    void VisibilityCommands::setScaleMaximum(int scaleMaximum) {
        delete this->scaleMaximum;
        this->scaleMaximum = new ScaleMaximum(scaleMaximum);
    }

    void VisibilityCommands::setId(const std::string& id) {
        delete this->id;
        this->id = new Id(id);
    }

    void VisibilityCommands::setParent(const std::string& parentId) {
        delete this->parent;
        this->parent = new Parent(parentId);
    }

    void VisibilityCommands::setHover(bool hover) {
        delete this->hover;
        this->hover = new Hover(hover);
    }

    void VisibilityCommands::execute() const
    {
        if (viewingGroup) viewingGroup->execute();
        if (displayPlane) displayPlane->execute();
        if (drawingPriority) drawingPriority->execute();
        if (scaleMinimum) scaleMinimum->execute();
        if (scaleMaximum) scaleMaximum->execute();
        if (id) id->execute();
        if (parent) parent->execute();
        if (hover) hover->execute();
    }

    void VisibilityCommands::parse(const std::string& key, std::string value)
    {
        if (key == "ViewingGroup")
        {
            //setViewingGroup();
        }
        else if (key == "DisplayPlane")
        {
            //setDisplayPlane();
        }
        else if (key == "DrawingPriority")
        {
            //setDrawingPriority();
        }
        else if (key == "ScaleMinimum")
        {
            //setScaleMinimum();
        }
        else if (key == "ScaleMaximum")
        {
            //setScaleMaximum();
        }
        else if (key == "Id")
        {
            //setId();
        }
        else if (key == "Parent")
        {
            //setParent();
        }
        else if (key == "Hover")
        {
            //setHover();
        }
    }

    // ViewingGroup class implementation
    ViewingGroup::ViewingGroup(const std::vector<std::string>& viewingGroups) : viewingGroups(viewingGroups) {}

    void ViewingGroup::init()
    {
        viewingGroups.clear();
    }

    void ViewingGroup::execute()
    {
    }

    void ViewingGroup::parse(const std::string& input) 
    {
		viewingGroups = LatLonUtility::Split(input, ",");
	}

    // DisplayPlane class implementation
    DisplayPlane::DisplayPlane(const std::string& displayPlane) : displayPlane(displayPlane) {}

    void DisplayPlane::init()
    {
		displayPlane = "";
    }

    void DisplayPlane::execute()  
    {
    }

    void DisplayPlane::parse(const std::string& input) 
    {
		displayPlane = input;
	}

    // DrawingPriority class implementation
    DrawingPriority::DrawingPriority(int drawingPriority) : drawingPriority(drawingPriority) {}

    void DrawingPriority::init()
    {
		drawingPriority = 0; // Default value
    }

    void DrawingPriority::execute()  {
    }

    void DrawingPriority::parse(const std::string& input) 
    {
		try 
        {
			drawingPriority = std::stoi(input);
		}
        catch (const std::exception& e) 
        {
            drawingPriority = 0; // Default value if parsing fails
        }
    }

    // ScaleMinimum class implementation
    ScaleMinimum::ScaleMinimum(int scaleMinimum) : scaleMinimum(scaleMinimum) {}

    void ScaleMinimum::init()
    {
        scaleMinimum = 0;
    }

    void ScaleMinimum::execute()  {
    }

    void ScaleMinimum::parse(const std::string& input) 
    {
        try
        {
            scaleMinimum = std::stoi(input);
        }
        catch (const std::exception& e)
        {
            scaleMinimum = 0; // Default value if parsing fails
        }
	}

    // ScaleMaximum class implementation
    ScaleMaximum::ScaleMaximum(int scaleMaximum) : scaleMaximum(scaleMaximum) {}

    void ScaleMaximum::init()
    {
        scaleMaximum = 0;
    }

    void ScaleMaximum::execute()  {
    }

    void ScaleMaximum::parse(const std::string& input) 
    {
        try
        {
            scaleMaximum = std::stoi(input);
        }
        catch (const std::exception& e)
        {
            scaleMaximum = 0; // Default value if parsing fails
        }
    }

    // Id class implementation
    Id::Id(const std::string& id) : id(id) {}

    void Id::init()
    {
        id = "";
    }

    void Id::execute()  {
    }

    void Id::parse(const std::string& input) 
    {
		id = input;
	}

    // Parent class implementation
    Parent::Parent(const std::string& parentId) : parentId(parentId) {}

    void Parent::init()
    {
        parentId = "";
    }

    void Parent::execute()  {
    }

    void Parent::parse(const std::string& input) 
    {
		parentId = input;
    }

    // Hover class implementation
    Hover::Hover(bool hover) : hover(hover) {}

    void Hover::init()
    {
        hover = false;
    }

    void Hover::execute()  {
    }

    void Hover::parse(const std::string& input) 
    {
		if (input == "true") 
        {
			hover = true;
        } 
        else if (input == "false")
        {
            hover = false;
        }
    }
}