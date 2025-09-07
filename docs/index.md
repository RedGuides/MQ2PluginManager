---
tags:
  - plugin
resource_link: "https://www.redguides.com/community/resources/mq2pluginmanager.1201/"
support_link: "https://www.redguides.com/community/threads/mq2pluginmanager.69624/"
repository: "https://github.com/RedGuides/MQ2PluginManager"
authors: "ChatWithThisName"
tagline: "Quickly view loaded and unloaded plugins, and one click load and unload of them."
acknowledgements: "dencelle's MQ2MacroTool"
---

# MQ2PluginManager

<!--desc-start-->
See a list of all your plugins in a nice, graphical user interface. Click them to load or unload.
<!--desc-end-->
* Plugins in Red are not loaded, plugins in green are loaded.
* Left click to load a plugin.
* Right click to unload a plugin.

The window may be closed. If you close it, type `/pluginman` to bring it back.


## Commands

<a href="cmd-pluginman/">
{% 
  include-markdown "projects/mq2pluginmanager/cmd-pluginman.md" 
  start="<!--cmd-syntax-start-->" 
  end="<!--cmd-syntax-end-->" 
%}
</a>
:    {% include-markdown "projects/mq2pluginmanager/cmd-pluginman.md" 
        start="<!--cmd-desc-start-->" 
        end="<!--cmd-desc-end-->" 
        trailing-newlines=false 
     %} {{ readMore('projects/mq2pluginmanager/cmd-pluginman.md') }}

## Troubleshooting

!!! question "I don't see the window!"
    If you're having trouble getting the window to load, try the following commands:

    ```
    /loadskin default
    /plugin mq2pluginmanager unload
    /plugin mq2pluginmanager load
    ```

