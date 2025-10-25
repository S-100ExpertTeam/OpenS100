#pragma once
#include "Command.h"

namespace DrawingCommand
{
    class ViewingGroup : public Command 
    {
    public:
		ViewingGroup() = default;
        ViewingGroup(const std::vector<std::string>& viewingGroups);

        Enum_CommandType GetType() const override;

		virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::vector<std::string> viewingGroups;

    public:
		std::vector<std::string> GetViewingGroups() const;
    };

    class DisplayPlane : public Command 
    {
    public:
		DisplayPlane() = default;
        DisplayPlane(const std::string& displayPlane);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string displayPlane;

    public:
		std::string GetDisplayPlane() const;
    };

    class DrawingPriority : public Command 
    {
    public:
		DrawingPriority() = default;
        DrawingPriority(int drawingPriority);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int drawingPriority = 0;

    public:
		int GetDrawingPriority() const;
    };

    class ScaleMinimum : public Command 
    {
    public:
		ScaleMinimum() = default;
        ScaleMinimum(int scaleMinimum);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMinimum = INT32_MAX;

    public:
		int GetScaleMinimum() const;
    };

    class ScaleMaximum : public Command 
    {
    public:
		ScaleMaximum() = default;
        ScaleMaximum(int scaleMaximum);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        int scaleMaximum = INT32_MIN;

	public:
		int GetScaleMaximum() const;
    };

    class Id : public Command 
    {
    public:
		Id() = default;
        Id(const std::string& id);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string id;

    public:
        std::string GetId();
    };

    class Parent : public Command 
    {
    public:
		Parent() = default;
        Parent(const std::string& parentId);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        std::string parentId;

    public:
		std::string GetParentId() const;
    };

    class Hover : public Command {
    public:
		Hover() = default;
        Hover(bool hover);

        Enum_CommandType GetType() const override;

        virtual void init() override;
        virtual void execute() override;
        virtual void parse(const std::string& input) override;

    private:
        bool hover = false;

    public:
		bool GetHover() const;
    };
}