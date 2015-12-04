This project implements scalable framework for modern TV and STB
devices. As an example, the Zapper application is implemented.

This project is part of PPuTViOS 1, a
course in E2, RT-RK undergraduate studies
at Faculty of Technical Sciences in Novi Sad.

Requirements (specification)
============================

The application had to meet several requirements:

  - Run on given STB (ARM based Marvell SoC)
  - Use remote control as input
  - It's native Linux application using the TDP API

Needed features are split into three  parts:

1. Basic features
  - Load inititial configuration. An example is given in the
    config.ini file, in this repository. Path to the config
    file is given as first command line argument.
  - Set NIM (frontend) parameters as in the config file. This
    means setting base frequency, bandwidth and DVB standard.
  - Parse PAT and PMT sections and extract service list with
    parameters needed for reproduction.
  - Enable remote controller support. Implement following functions:
    - channel change
    - volume control
    - exit

2. Advanced features
  - Jump directly to channel by number
  - On each channel change, show info dialog with
    informations about current channel and information
    if teletext is available. This dialog should automatically
    disappear after 3 seconds. Info dialog can be made visible
    again by pressing the info button.

3. Custom feature
  - Parse SDT section and extract information about service type
    and service name.

Basic features and advanced features were common for all students,
and custom feature was different for every one of us.

Introduction
============

Although main target was to implement zapper application,
implicit target was to build a framework which will make this
job easier. This implementation is based on MVC pattern plus
separate input block. This pattern was chosen because it scales
well and can support multiple platforms while still saving
a lot of reusable code.

Model
------

In this case, model is an abstraction of a real device. STB
can be in some state and can access some data. Model is an interface
which enables user (developer) to change state of STB or access
some of that data. Examples of state changes are triggering
service scan, changing channel, changing volume level etc. Available
data which can be accessed can be list of available services,
current volume, current channel...

Model is just an interface. Every module which implements
this interface can be used as model part of this MVC implementation.
This means that we can support several platform or test on
virtual platforms.


View
----

View part is dedicated to the abstraction of zapper graphics UI
elements. By defining view as an interface, this framework
can support a range of graphic and console libraries.
It was experimented with basic serial console, DirectFB and OpenGL.
Final application was using DirectFB implementation.

View should support rendering of informations about current
channel and volume informations. This should be as easy as
passing `service_item_t` object to view's handler.

Controller
----------

Controller connects model and view. It was an experiment in
implementation of FSM (finite state machine). The most elegant
solution was chosen for final implementation. The FSM is event based
and source of events is input device (keyboard or remote controller)
although another input was considered (model) for events like
"scan finished", "zapp finished" etc...

Input
------

Input is an abstraction of input devices. Once the callback is
registered (controller's input event handler), every event (key press)
results with calling the callback with normalized key codes.

Implemented solution
====================

Inline documentation is provided as needed for detailed description
of implementation. This is just an overview of main issues which
this implementation was trying to solve.

Blocking input
--------------
  Reading input is a blocking operation. To decouple input, model
  and UI, input devices are always running in separate thread.

Controller states
-----------------
  Controller FSM implements following states:
    - Initialization
    - Idle
    - Number input
    - Stop

  Input handler will decode normalized key code, and choose
  which signal should be fired and if there are some arguments
  to be passed along.

  Initialization - initialize STB model
  Idle - handle input events (main entry point)
  Number input - support 3-num input
  Stop - deinit STB model

View render loop
----------------
  UI render loop is also running in it's own thread. Every time
  controller needs to change UI state, it will call a view callback
  with appropriate arguments. Callback will set "last known data"
  in view's private area (static global variables in view module).

  When "show" callback is triggered, it will set the "visible" flag.
  Render routines for each UI element will on each frame render
  check status of this flag and render the element only if it is
  visible. The flag can be reset by calling appropriate "hide"
  callback, probably from controller.

UI control
---------------------------------
  Although render routines in view module implement exact procedure
  for rendering an UI element, controller part of MVC is in charge
  of changing UI state. Here is an example which can be examined
  in the controller.c if more details are needed:

  - When application is in the *idle* state and P+ key is pressed,
    idle state handler will get SIG_NUM signal with P+ value.
    After that, it will call view's show_info_bar routine and
    schedule hide_info_bar to execute after 3 seconds. Of course,
    it will call appropriate model function for channel change and
    will stay in the idle state.

Service list abstraction
------------------------
  The main purpose of model is to play some A/V streams. That
  streams can be received from a DVB stream, but also can be read
  from a file system, a camera or anything else. User (watcher) will
  want see list of available services, and switch between them.

  Model should hide whole process of getting that informations and
  in case of DVB set-top-box service list is read from DVB stream
  by parsing PAT and SDT tables and joining that data together
  by program number. Accessing raw PAT and SDT tables with variable
  lenghts can be impractical in many cases. In some other situations
  there aren't even PAT and SDT tables (reading folder with files).
  For that issue the service list abstraction is introduced.
  Model is in charge of filling that structure with data.

  Note: in this implementation, SDT is still not tested and not in
  usable state.

  For playing some service in case of DVB every service consists
  of several streams. PIDs of that streams are in PMT sections.
  Procedure of setting demux filter, parsing PMT, extraction of
  A/V stream PIDs and setting those PIDs to player are also hidden
  from model's user (controller).

Future development
==================

This architecture shows some pros and cons. It certainly scales well
and can be extended to support more features and platforms but
introduces large codebase which extends development time. Suggestions
mentioned here are of two kinds: improvement of this architecture and
suggesting new, smaller but not worse architecture.

Improvements
------------

Abstract framebuffer interface can help in code reuse of view part.
Currently, every new implementation of view needs whole code rewrite
but AFB would require just writing wrapper for some basic GUI
operations like drawing a rectangle or clearing screen.

Autohide element. It's a common pattern to have autohide elements
like Android toasts. Currently, controller is in charge for scheduling
call to view's hide callback and this means explicit definition
of timer and setting it's trigger time every time when we show
UI element. If UI elements were able to hide themselves code would
be shorter and eaiser for maintenance.

New architecture
----------------

This architecture is quite big in sense of overheads. Some
alternatives are modern variations of classic MVC, like MV*
(MV-something/start). MV* pattern doesn't reuire so strict
controller. That way, callbacks could be interconnected more
tightly and FSM as event processor wouldn't be needed.
A good example of this pattern is in AngularJS and it's
worth of consideration.
