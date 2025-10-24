#include "stdafx.h"
#include "VisibilityCommands.h"

#include "..\\LatLonUtility\\LatLonUtility.h"

namespace DrawingCommand
{
    // ViewingGroup class implementation
    ViewingGroup::ViewingGroup(const std::vector<std::string>& viewingGroups) : viewingGroups(viewingGroups) {}

    Enum_StateCommand ViewingGroup::GetType() const
    {
		return Enum_StateCommand::ViewingGroup;
    }

    void ViewingGroup::init()
    {
        Command::init();
        viewingGroups.clear();
    }

    void ViewingGroup::execute()
    {
    }

    void ViewingGroup::parse(const std::string& input) 
    {
        setPresent();
        // ViewingGroup:viewingGroup[,viewingGroup2[,¡¦]] 
		viewingGroups = LatLonUtility::Split(input, ",");
	}

    std::vector<std::string> ViewingGroup::GetViewingGroups() const
    {
		return viewingGroups;
    }

    // DisplayPlane class implementation
    DisplayPlane::DisplayPlane(const std::string& displayPlane) : displayPlane(displayPlane) {}

    Enum_StateCommand DisplayPlane::GetType() const
    {
		return Enum_StateCommand::DisplayPlane;
    }

    void DisplayPlane::init()
    {
        Command::init();
		displayPlane.clear();
    }

    void DisplayPlane::execute()  
    {
    }

    void DisplayPlane::parse(const std::string& input) 
    {
        setPresent();
        // DisplayPlane:displayPlane
		displayPlane = input;
	}

    std::string DisplayPlane::GetDisplayPlane() const
	{
		return displayPlane;
	}

    // DrawingPriority class implementation
    DrawingPriority::DrawingPriority(int drawingPriority) : drawingPriority(drawingPriority) {}

    Enum_StateCommand DrawingPriority::GetType() const
    {
        return Enum_StateCommand::DrawingPriority;
    }

    void DrawingPriority::init()
    {
        Command::init();
		drawingPriority = 0; // Default value
    }

    void DrawingPriority::execute()  {
    }

    void DrawingPriority::parse(const std::string& input) 
    {
        setPresent();
        // DrawingPriority:drawingPriority 

		try 
        {
			drawingPriority = std::stoi(input);
		}
        catch (const std::exception& e) 
        {
            drawingPriority = 0; // Default value if parsing fails
        }
    }

    int DrawingPriority::GetDrawingPriority() const
    {
		return drawingPriority;
    }

    // ScaleMinimum class implementation
    ScaleMinimum::ScaleMinimum(int scaleMinimum) : scaleMinimum(scaleMinimum) {}

    Enum_StateCommand ScaleMinimum::GetType() const
    {
        return Enum_StateCommand::ScaleMinimum;
    }

    void ScaleMinimum::init()
    {
        Command::init();
        scaleMinimum = 0;
    }

    void ScaleMinimum::execute()  {
    }

    void ScaleMinimum::parse(const std::string& input) 
    {
        setPresent();
        // ScaleMinimum:scaleMinimum

        try
        {
            scaleMinimum = std::stoi(input);
        }
        catch (const std::exception& e)
        {
            scaleMinimum = 0; // Default value if parsing fails
        }
	}

    int ScaleMinimum::GetScaleMinimum() const
    {
        return scaleMinimum;
    }

    // ScaleMaximum class implementation
    ScaleMaximum::ScaleMaximum(int scaleMaximum) : scaleMaximum(scaleMaximum) {}

    Enum_StateCommand ScaleMaximum::GetType() const
    {
        return Enum_StateCommand::ScaleMaximum;
    }

    void ScaleMaximum::init()
    {
        Command::init();
        scaleMaximum = 0;
    }

    void ScaleMaximum::execute()  {
    }

    void ScaleMaximum::parse(const std::string& input) 
    {
        setPresent();
        // ScaleMaximum:scaleMaximum 

        try
        {
            scaleMaximum = std::stoi(input);
        }
        catch (const std::exception& e)
        {
            scaleMaximum = 0; // Default value if parsing fails
        }
    }

    int ScaleMaximum::GetScaleMaximum() const
    {
        return scaleMaximum;
	}

    // Id class implementation
    Id::Id(const std::string& id) : id(id) {}

    Enum_StateCommand Id::GetType() const
    {
        return Enum_StateCommand::Id;
    }

    void Id::init()
    {
        Command::init();
        id.clear();
    }

    void Id::execute()  {
    }

    void Id::parse(const std::string& input) 
    {
        setPresent();
        // Id[:id]
		id = input;
	}

    std::string Id::GetId()
    {
        return id;
    }

    // Parent class implementation
    Parent::Parent(const std::string& parentId) : parentId(parentId) {}

    Enum_StateCommand Parent::GetType() const
    {
        return Enum_StateCommand::Parent;
    }

    void Parent::init()
    {
        Command::init();
        parentId.clear();
    }

    void Parent::execute()  {
    }

    void Parent::parse(const std::string& input) 
    {
        setPresent();
        // Parent[:id]
		parentId = input;
    }

    std::string Parent::GetParentId() const
    {
		return parentId;
    }

    // Hover class implementation
    Hover::Hover(bool hover) : hover(hover) {}

    Enum_StateCommand Hover::GetType() const
    {
        return Enum_StateCommand::Hover;
    }

    void Hover::init()
    {
        Command::init();
        hover = false;
    }

    void Hover::execute()  {
    }

    void Hover::parse(const std::string& input) 
    {
        setPresent();
        // Hover:hover 

		if (input == "true") 
        {
			hover = true;
        } 
        else if (input == "false")
        {
            hover = false;
        }
    }

    bool Hover::GetHover() const
    {
		return hover;
    }
}