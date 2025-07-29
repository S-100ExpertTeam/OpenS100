#pragma once
#include "StateCommand.h"

namespace DrawingInstructions
{
    class ViewingGroup : public StateCommand 
    {
    public:
		ViewingGroup() = default;
        ViewingGroup(const std::vector<std::string>& viewingGroups);
		virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> viewingGroups;

    public:
		std::vector<std::string> GetViewingGroups() const;
    };

    class DisplayPlane : public StateCommand 
    {
    public:
		DisplayPlane() = default;
        DisplayPlane(const std::string& displayPlane);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string displayPlane;

    public:
		std::string GetDisplayPlane() const;
    };

    class DrawingPriority : public StateCommand 
    {
    public:
		DrawingPriority() = default;
        DrawingPriority(int drawingPriority);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int drawingPriority = 0;

    public:
		int GetDrawingPriority() const;
    };

    class ScaleMinimum : public StateCommand 
    {
    public:
		ScaleMinimum() = default;
        ScaleMinimum(int scaleMinimum);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMinimum = INT32_MAX;

    public:
		int GetScaleMinimum() const;
    };

    class ScaleMaximum : public StateCommand 
    {
    public:
		ScaleMaximum() = default;
        ScaleMaximum(int scaleMaximum);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMaximum = INT32_MIN;
    };

    class Id : public StateCommand 
    {
    public:
		Id() = default;
        Id(const std::string& id);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string id;

    public:
        std::string GetId();
    };

    class Parent : public StateCommand 
    {
    public:
		Parent() = default;
        Parent(const std::string& parentId);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string parentId;

    public:
		std::string GetParentId() const;
    };

    class Hover : public StateCommand {
    public:
		Hover() = default;
        Hover(bool hover);
        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        bool hover = false;

    public:
		bool GetHover() const;
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

