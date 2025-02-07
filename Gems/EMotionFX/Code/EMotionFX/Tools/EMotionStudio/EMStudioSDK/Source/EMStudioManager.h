/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

// include MCore
#include <MCore/Source/Color.h>
#include <MCore/Source/CommandGroup.h>
#include <MCore/Source/CommandManagerCallback.h>

// include the command system
#include <EMotionFX/CommandSystem/Source/CommandManager.h>

// include the gizmos
#include <EMotionFX/Rendering/Common/TransformationManipulator.h>

// include the EMStudio Config
#include "EMStudioConfig.h"
#include <MysticQt/Source/MysticQtManager.h>
#include "PluginManager.h"
#include "LayoutManager.h"
#include "NotificationWindowManager.h"
#include "Workspace.h"
#include "MainWindow.h"
#include <Source/Editor/Plugins/SkeletonOutliner/SkeletonOutlinerBus.h>

// include Qt
#include <QString>
#include <QPainter>
#include <QPointer>
#include <QWidget>

// forward declarations
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QLabel)


namespace EMStudio
{
    // forward declarations
    class MotionEventPresetManager;

    /**
     *
     *
     *
     */
    class EMSTUDIO_API EMStudioManager
        : private EMotionFX::SkeletonOutlinerNotificationBus::Handler
    {
        MCORE_MEMORYOBJECTCATEGORY(EMStudioManager, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_EMSTUDIOSDK)

    public:
        EMStudioManager(QApplication* app, int& argc, char* argv[]);
        ~EMStudioManager();

        const char* GetCompileDate() const                                      { return m_compileDate.c_str(); }

        MCORE_INLINE QApplication* GetApp()                                     { return m_app; }
        MCORE_INLINE bool HasMainWindow() const                                 { return !m_mainWindow.isNull(); }
        MainWindow* GetMainWindow();
        MCORE_INLINE PluginManager* GetPluginManager()                          { return m_pluginManager; }
        MCORE_INLINE LayoutManager* GetLayoutManager()                          { return m_layoutManager; }
        MCORE_INLINE NotificationWindowManager* GetNotificationWindowManager()  { return m_notificationWindowManager; }
        MCORE_INLINE CommandSystem::CommandManager* GetCommandManager()         { return m_commandManager; }
        AZStd::string GetAppDataFolder() const;
        AZStd::string GetRecoverFolder() const;
        AZStd::string GetAutosavesFolder() const;

        // text rendering helper function
        static void RenderText(QPainter& painter, const QString& text, const QColor& textColor, const QFont& font, const QFontMetrics& fontMetrics, Qt::Alignment textAlignment, const QRect& rect);

        // motion event presets
        MotionEventPresetManager* GetEventPresetManger() const                  { return m_eventPresetManager; }

        void SetAutoLoadLastWorkspace(bool autoLoad)                            { m_autoLoadLastWorkspace = autoLoad; }
        bool GetAutoLoadLastWorkspace() const                                   { return m_autoLoadLastWorkspace; }

        const char* ConstructHTMLLink(const char* text, const MCore::RGBAColor& color = MCore::RGBAColor(0.95315f, 0.609375f, 0.109375f));
        void SetWidgetAsInvalidInput(QWidget* widget);

        static void MakeTransparentButton(QToolButton* button, const char* iconFileName, const char* toolTipText, uint32 width = 20, uint32 height = 20);
        static void MakeTransparentButton(QPushButton* button, const char* iconFileName, const char* toolTipText, uint32 width = 20, uint32 height = 20);
        static void MakeTransparentMenuButton(QPushButton* button, const char* iconFileName, const char* toolTipText, uint32 width = 30, uint32 height = 20);
        static QLabel* MakeSeperatorLabel(uint32 width, uint32 height);

        int ExecuteApp();
        void LogInfo();

        // in case the array is empty, all nodes are shown
        void SetVisibleJointIndices(const AZStd::unordered_set<size_t>& visibleJointIndices);
        const AZStd::unordered_set<size_t>& GetVisibleJointIndices() const                                 { return m_visibleJointIndices; }

        void SetSelectedJointIndices(const AZStd::unordered_set<size_t>& selectedJointIndices);
        const AZStd::unordered_set<size_t>& GetSelectedJointIndices() const                                { return m_selectedJointIndices; }

        Workspace* GetWorkspace()                                                               { return &m_workspace; }

        // functions for adding/removing gizmos
        MCommon::TransformationManipulator* AddTransformationManipulator(MCommon::TransformationManipulator* manipulator);
        void RemoveTransformationManipulator(MCommon::TransformationManipulator* manipulator);
        AZStd::vector<MCommon::TransformationManipulator*>* GetTransformationManipulators();

        void ClearScene();  // remove animgraphs, animgraph instances and actors

        MCORE_INLINE bool GetAvoidRendering() const                                             { return m_avoidRendering; }
        MCORE_INLINE void SetAvoidRendering(bool avoidRendering)                                { m_avoidRendering = avoidRendering; }
        MCORE_INLINE bool GetIgnoreVisibility() const                                           { return m_ignoreVisible; }
        MCORE_INLINE void SetIgnoreVisibility(bool ignoreVisible)                               { m_ignoreVisible = ignoreVisible; }
        MCORE_INLINE bool GetSkipSourceControlCommands()                                        { return m_skipSourceControlCommands; }
        MCORE_INLINE void SetSkipSourceControlCommands(bool skip)                               { m_skipSourceControlCommands = skip; }
    private:
        AZStd::vector<MCommon::TransformationManipulator*> m_transformationManipulators;
        QPointer<MainWindow>                m_mainWindow;
        QApplication*                       m_app;
        PluginManager*                      m_pluginManager;
        LayoutManager*                      m_layoutManager;
        NotificationWindowManager*          m_notificationWindowManager;
        CommandSystem::CommandManager*      m_commandManager;
        AZStd::string                       m_compileDate;
        AZStd::unordered_set<size_t>       m_visibleJointIndices;
        AZStd::unordered_set<size_t>       m_selectedJointIndices;
        Workspace                           m_workspace;
        bool                                m_autoLoadLastWorkspace;
        AZStd::string                       m_htmlLinkString;
        bool                                m_avoidRendering;
        bool                                m_ignoreVisible = false;
        MotionEventPresetManager*           m_eventPresetManager;
        bool                                m_skipSourceControlCommands = false;

        // SkeletonOutlinerNotificationBus
        void JointSelectionChanged();

        class EMSTUDIO_API EventProcessingCallback
            : public MCore::CommandManagerCallback
        {
            MCORE_MEMORYOBJECTCATEGORY(EventProcessingCallback, MCore::MCORE_DEFAULT_ALIGNMENT, MEMCATEGORY_EMSTUDIOSDK);

        public:
            void OnPreExecuteCommand(MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine) override;
            void OnPostExecuteCommand(MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine, bool wasSuccess, const AZStd::string& outResult) override;
            void OnPreExecuteCommandGroup(MCore::CommandGroup* group, bool undo) override                                                                                                       { MCORE_UNUSED(group); MCORE_UNUSED(undo); }
            void OnPostExecuteCommandGroup(MCore::CommandGroup* group, bool wasSuccess) override                                                                                                { MCORE_UNUSED(group); MCORE_UNUSED(wasSuccess); }
            void OnAddCommandToHistory(size_t historyIndex, MCore::CommandGroup* group, MCore::Command* command, const MCore::CommandLine& commandLine) override                                { MCORE_UNUSED(historyIndex); MCORE_UNUSED(group); MCORE_UNUSED(command); MCORE_UNUSED(commandLine); }
            void OnRemoveCommand(size_t historyIndex) override                                                                                                                                  { MCORE_UNUSED(historyIndex); }
            void OnSetCurrentCommand(size_t index) override                                                                                                                                     { MCORE_UNUSED(index); }
        };
        EventProcessingCallback*    m_eventProcessingCallback;
    };


    /**
     *
     *
     *
     */
    class EMSTUDIO_API Initializer
    {
    public:
        static bool MCORE_CDECL Init(QApplication* app, int& argc, char* argv[]);
        static void MCORE_CDECL Shutdown();
    };


    // the global manager
    extern EMSTUDIO_API EMStudioManager* gEMStudioMgr;

    // shortcuts
    MCORE_INLINE QApplication*                  GetApp()                        { return gEMStudioMgr->GetApp(); }
    MCORE_INLINE EMStudioManager*               GetManager()                    { return gEMStudioMgr; }
    MCORE_INLINE bool                           HasMainWindow()                 { return gEMStudioMgr->HasMainWindow(); }
    MCORE_INLINE MainWindow*                    GetMainWindow()                 { return gEMStudioMgr->GetMainWindow(); }
    MCORE_INLINE PluginManager*                 GetPluginManager()              { return gEMStudioMgr->GetPluginManager(); }
    MCORE_INLINE LayoutManager*                 GetLayoutManager()              { return gEMStudioMgr->GetLayoutManager(); }
    MCORE_INLINE NotificationWindowManager*     GetNotificationWindowManager()  { return gEMStudioMgr->GetNotificationWindowManager(); }
    MCORE_INLINE MotionEventPresetManager*      GetEventPresetManager()         { return gEMStudioMgr->GetEventPresetManger(); }
    MCORE_INLINE CommandSystem::CommandManager* GetCommandManager()             { return gEMStudioMgr->GetCommandManager(); }
}   // namespace EMStudio
