#pragma once
#include "StateCommand.h"

namespace DrawingInstructions
{
    class ViewingGroup : public StateCommand 
    {
    public:
        ViewingGroup(const std::vector<std::string>& viewingGroups);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> viewingGroups;
    };

    class DisplayPlane : public StateCommand {
    public:
        DisplayPlane(const std::string& displayPlane);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string displayPlane;
    };

    class DrawingPriority : public StateCommand {
    public:
        DrawingPriority(int drawingPriority);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int drawingPriority;
    };

    class ScaleMinimum : public StateCommand {
    public:
        ScaleMinimum(int scaleMinimum);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMinimum;
    };

    class ScaleMaximum : public StateCommand {
    public:
        ScaleMaximum(int scaleMaximum);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMaximum;
    };

    class Id : public StateCommand {
    public:
        Id(const std::string& id);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string id;
    };

    class Parent : public StateCommand {
    public:
        Parent(const std::string& parentId);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string parentId;
    };

    class Hover : public StateCommand {
    public:
        Hover(bool hover);
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        bool hover;
    };




    class VisibilityCommands 
    {
    public:
		VisibilityCommands() = default;
		VisibilityCommands(const VisibilityCommands&) = delete;
		VisibilityCommands& operator=(const VisibilityCommands&) = delete;
		VisibilityCommands(VisibilityCommands&&) = delete;
		VisibilityCommands& operator=(VisibilityCommands&&) = delete;
		~VisibilityCommands();

		void setViewingGroup(const std::vector<std::string>& viewingGroups);
		void setDisplayPlane(const std::string& displayPlane);
		void setDrawingPriority(int drawingPriority);
		void setScaleMinimum(int scaleMinimum);
		void setScaleMaximum(int scaleMaximum);
		void setId(const std::string& id);
		void setParent(const std::string& parentId);
		void setHover(bool hover);

        void parse(const std::string& key, std::string value);
		void execute() const;
        

    private:
        //Visibility commands
        DrawingInstructions::ViewingGroup* viewingGroup = nullptr;
        DrawingInstructions::DisplayPlane* displayPlane = nullptr;
        DrawingPriority* drawingPriority = nullptr;
        ScaleMinimum* scaleMinimum = nullptr;
        ScaleMaximum* scaleMaximum = nullptr;
        Id* id = nullptr;
        Parent* parent = nullptr;
        Hover* hover = nullptr;
    };

}

