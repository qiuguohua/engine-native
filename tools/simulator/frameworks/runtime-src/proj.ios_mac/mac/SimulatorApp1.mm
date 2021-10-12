/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SimulatorApp.h"

#import <Cocoa/Cocoa.h>
#import "ConsoleWindowController.h"

#include <string>
#include "ProjectConfig/ProjectConfig.h"
#include "ProjectConfig/SimulatorConfig.h"
#include "SimulatorApp1.h"
#include "Game.h"

@interface AppController : NSObject <NSApplicationDelegate, NSWindowDelegate, NSFileManagerDelegate>
{
    NSWindow *_window;
    NSMenu *menu;
    
    Game *_app;
    ProjectConfig _project;
    int _debugLogFile;
    std::string _entryPath;
    
    //log file
    ConsoleWindowController *_consoleController;
    NSFileHandle *_fileHandle;
    
    //console pipe
    NSPipe *_pipe;
    NSFileHandle *_pipeReadHandle;
}

@property (nonatomic, assign) IBOutlet NSMenu* menu;

-(BOOL)application:(NSApplication*)app openFile:(NSString*)path;
-(IBAction)onFileClose:(id)sender;
-(IBAction)onWindowAlwaysOnTop:(id)sender;
-(NSWindow*)getWindow;
@end


static id SIMULATOR = nullptr;
@implementation AppController

@synthesize menu;

- (void) parseCocosProjectConfig:(ProjectConfig*)config
{
    // get project directory
    ProjectConfig tmpConfig;
    NSArray *nsargs = [[NSProcessInfo processInfo] arguments];
    long n = [nsargs count];
    if (n >= 2)
    {
        std::vector<string> args;
        for (int i = 0; i < [nsargs count]; ++i)
        {
            string arg = [[nsargs objectAtIndex:i] cStringUsingEncoding:NSUTF8StringEncoding];
            if (arg.length()) args.push_back(arg);
        }

        if (args.size() && args.at(1).at(0) == '/')
        {
            // IDEA:
            // for Code IDE before RC2
            tmpConfig.setProjectDir(args.at(1));
        }

        tmpConfig.parseCommandLine(args);
    }

    // set project directory as search root path
    string solutionDir = tmpConfig.getProjectDir();
    string spath = solutionDir;
    if (!solutionDir.empty())
    {
        for (int i = 0; i < solutionDir.size(); ++i)
        {
            if (solutionDir[i] == '\\')
            {
                solutionDir[i] = '/';
            }
        }

        spath = solutionDir;
        if (spath[spath.length() - 1] == '/') {
            spath = spath.substr(0, spath.length() - 1);
        }
        string strExtention = FileUtils::getInstance()->getFileExtension(spath);
        int pos = -1;
        if(strExtention.compare(".csd") == 0)
        {
            pos = spath.rfind('/');
            if(pos > 0)
                spath = spath.substr(0, pos);
        }
        pos = spath.rfind('/');
        if(pos > 0)
            spath = spath.substr(0, pos+1);
        FileUtils::getInstance()->addSearchPath(spath);

        FileUtils::getInstance()->setDefaultResourceRootPath(solutionDir);
        FileUtils::getInstance()->addSearchPath(solutionDir);
        FileUtils::getInstance()->addSearchPath(tmpConfig.getProjectDir());
    }
    else
    {
        FileUtils::getInstance()->setDefaultResourceRootPath(tmpConfig.getProjectDir());
    }

    // parse config.json when firstly init ConfigParser single instance
    auto parser = ConfigParser::getInstance();

    // set information
    config->setConsolePort(parser->getConsolePort());
    config->setFileUploadPort(parser->getUploadPort());
    config->setFrameSize(parser->getInitViewSize());
    if (parser->isLanscape())
    {
        config->changeFrameOrientationToLandscape();
    }
    else
    {
        config->changeFrameOrientationToPortait();
    }
    config->setScriptFile(parser->getEntryFile());
}

@end

namespace {
  AppController* app;
}

SimulatorWin::SimulatorWin() {
    app = [[AppController alloc] init];
}

SimulatorWin::~SimulatorWin() {

}

int SimulatorWin::run() {
    SIMULATOR = self;
    player::PlayerMac::create();

    _debugLogFile = 0;
}