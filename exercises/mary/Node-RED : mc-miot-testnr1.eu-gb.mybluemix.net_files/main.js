/**
 * Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/
!function(){function a(){$.ajax({headers:{Accept:"application/json"},cache:!1,url:"nodes",success:function(a){RED.nodes.setNodeList(a);for(var c=0,d=0;d<a.length;d++){var e=a[d];"node-red"!=e.module&&(c++,RED.i18n.loadCatalog(e.id,function(){c--,0===c&&b()}))}0===c&&b()}})}function b(){$.ajax({headers:{Accept:"text/html"},cache:!1,url:"nodes",success:function(a){$("body").append(a),$("body").i18n(),$("#palette > .palette-spinner").hide(),$(".palette-scroll").removeClass("hide"),$("#palette-search").removeClass("hide"),c()}})}function c(){$.ajax({headers:{Accept:"application/json"},cache:!1,url:"flows",success:function(a){var b=window.location.hash;RED.nodes.version(a.rev),RED.nodes.import(a.flows),RED.nodes.dirty(!1),RED.view.redraw(!0),/^#flow\/.+$/.test(b)&&RED.workspaces.show(b.substring(6));var c={};RED.comms.subscribe("notification/#",function(a,b){var d=a.split("/"),e=d[1];if(b.text){var f=RED._(b.text,{default:b.text});c.hasOwnProperty(e)?c[e].update(f,b.timeout):c[e]=RED.notify(f,b.type,void 0===b.timeout,b.timeout)}else c.hasOwnProperty(e)&&(c[e].close(),delete c[e])}),RED.comms.subscribe("status/#",function(a,b){var c=a.split("/"),d=RED.nodes.node(c[1]);d&&(b.hasOwnProperty("text")&&(b.text=d._(b.text.toString(),{defaultValue:b.text.toString()})),d.status=b,d.dirty=!0,RED.view.redraw())}),RED.comms.subscribe("node/#",function(a,b){var c,d,e,f;if("node/added"==a){var g=[];b.forEach(function(a){var b=a.id;RED.nodes.addNodeSet(a),g=g.concat(a.types),RED.i18n.loadCatalog(b,function(){$.get("nodes/"+b,function(a){$("body").append(a)})})}),g.length&&(e="<ul><li>"+g.join("</li><li>")+"</li></ul>",RED.notify(RED._("palette.event.nodeAdded",{count:g.length})+e,"success"))}else if("node/removed"==a)for(c=0;c<b.length;c++)d=b[c],f=RED.nodes.removeNodeSet(d.id),f.added&&(e="<ul><li>"+d.types.join("</li><li>")+"</li></ul>",RED.notify(RED._("palette.event.nodeRemoved",{count:d.types.length})+e,"success"));else"node/enabled"==a?b.types&&(f=RED.nodes.getNodeSet(b.id),f.added?(RED.nodes.enableNodeSet(b.id),e="<ul><li>"+b.types.join("</li><li>")+"</li></ul>",RED.notify(RED._("palette.event.nodeEnabled",{count:b.types.length})+e,"success")):$.get("nodes/"+b.id,function(a){$("body").append(a),e="<ul><li>"+b.types.join("</li><li>")+"</li></ul>",RED.notify(RED._("palette.event.nodeAdded",{count:b.types.length})+e,"success")})):"node/disabled"==a&&b.types&&(RED.nodes.disableNodeSet(b.id),e="<ul><li>"+b.types.join("</li><li>")+"</li></ul>",RED.notify(RED._("palette.event.nodeDisabled",{count:b.types.length})+e,"success"));RED.library.loadFlowLibrary()})}})}function d(){$.get("red/about",function(a){var b='<div style="text-align:center;"><img width="50px" src="red/images/node-red-icon.svg" /></div>';RED.sidebar.info.set(b+marked(a)),RED.sidebar.info.show()})}function e(){var b=[];b.push({id:"menu-item-view-menu",label:RED._("menu.label.view.view"),options:[{id:"menu-item-view-show-grid",label:RED._("menu.label.view.showGrid"),toggle:!0,onselect:"core:toggle-show-grid"},{id:"menu-item-view-snap-grid",label:RED._("menu.label.view.snapGrid"),toggle:!0,onselect:"core:toggle-snap-grid"},{id:"menu-item-status",label:RED._("menu.label.displayStatus"),toggle:!0,onselect:"core:toggle-status",selected:!0},null,{id:"menu-item-sidebar",label:RED._("menu.label.sidebar.show"),toggle:!0,onselect:"core:toggle-sidebar",selected:!0}]}),b.push(null),b.push({id:"menu-item-import",label:RED._("menu.label.import"),options:[{id:"menu-item-import-clipboard",label:RED._("menu.label.clipboard"),onselect:"core:show-import-dialog"},{id:"menu-item-import-library",label:RED._("menu.label.library"),options:[]}]}),b.push({id:"menu-item-export",label:RED._("menu.label.export"),disabled:!0,options:[{id:"menu-item-export-clipboard",label:RED._("menu.label.clipboard"),disabled:!0,onselect:"core:show-export-dialog"},{id:"menu-item-export-library",label:RED._("menu.label.library"),disabled:!0,onselect:"core:library-export"}]}),b.push(null),b.push({id:"menu-item-search",label:RED._("menu.label.search"),onselect:"core:search"}),b.push(null),b.push({id:"menu-item-config-nodes",label:RED._("menu.label.displayConfig"),onselect:"core:show-config-tab"}),b.push({id:"menu-item-workspace",label:RED._("menu.label.flows"),options:[{id:"menu-item-workspace-add",label:RED._("menu.label.add"),onselect:"core:add-flow"},{id:"menu-item-workspace-edit",label:RED._("menu.label.rename"),onselect:"core:edit-flow"},{id:"menu-item-workspace-delete",label:RED._("menu.label.delete"),onselect:"core:remove-flow"}]}),b.push({id:"menu-item-subflow",label:RED._("menu.label.subflows"),options:[{id:"menu-item-subflow-create",label:RED._("menu.label.createSubflow"),onselect:"core:create-subflow"},{id:"menu-item-subflow-convert",label:RED._("menu.label.selectionToSubflow"),disabled:!0,onselect:"core:convert-to-subflow"}]}),b.push(null),RED.settings.theme("palette.editable")!==!1&&(RED.palette.editor.init(),b.push({id:"menu-item-edit-palette",label:RED._("menu.label.editPalette"),onselect:"core:manage-palette"}),b.push(null)),b.push({id:"menu-item-keyboard-shortcuts",label:RED._("menu.label.keyboardShortcuts"),onselect:"core:show-help"}),b.push({id:"menu-item-show-tips",label:RED._("menu.label.showTips"),toggle:!0,selected:!0,onselect:"core:toggle-show-tips"}),b.push({id:"menu-item-help",label:RED.settings.theme("menu.menu-item-help.label","Node-RED website"),href:RED.settings.theme("menu.menu-item-help.url","http://nodered.org/docs")}),b.push({id:"menu-item-node-red-version",label:"v"+RED.settings.version,onselect:"core:show-about"}),RED.user.init(),RED.library.init(),RED.palette.init(),RED.sidebar.init(),RED.subflow.init(),RED.workspaces.init(),RED.clipboard.init(),RED.search.init(),RED.view.init(),RED.editor.init(),RED.keyboard.init(),RED.diff.init(),RED.menu.init({id:"btn-sidemenu",options:b}),RED.deploy.init(RED.settings.theme("deployButton",null)),RED.actions.add("core:show-about",d),RED.comms.connect(),$("#main-container").show(),$(".header-toolbar").show(),a()}$(function(){"localhost"!==window.location.hostname&&"127.0.0.1"!==window.location.hostname&&(document.title=document.title+" : "+window.location.hostname),ace.require("ace/ext/language_tools"),RED.i18n.init(function(){RED.settings.init(e)})})}();