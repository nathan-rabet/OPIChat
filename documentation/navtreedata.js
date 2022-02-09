/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "OPIchat", "index.html", [
    [ "OPIchat architecture bible", "md__a_r_c_h.html", [
      [ "Coding style", "md__a_r_c_h.html#autotoc_md1", null ],
      [ "Architecture", "md__a_r_c_h.html#autotoc_md2", null ],
      [ "Header files", "md__a_r_c_h.html#autotoc_md3", null ],
      [ "Isolation and \"foldering\"", "md__a_r_c_h.html#autotoc_md4", null ],
      [ "Documentation", "md__a_r_c_h.html#autotoc_md5", null ],
      [ "Unit testing", "md__a_r_c_h.html#autotoc_md6", null ],
      [ "Memory allocations", "md__a_r_c_h.html#autotoc_md7", null ],
      [ "Recommended practices", "md__a_r_c_h.html#autotoc_md8", null ]
    ] ],
    [ "Protocol", "md__p_r_o_t_o_c_o_l.html", [
      [ "Introduction", "md__p_r_o_t_o_c_o_l.html#autotoc_md10", null ],
      [ "Syntax", "md__p_r_o_t_o_c_o_l.html#autotoc_md11", null ],
      [ "Core features", "md__p_r_o_t_o_c_o_l.html#autotoc_md12", [
        [ "PING", "md__p_r_o_t_o_c_o_l.html#autotoc_md13", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md14", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md15", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md16", null ]
        ] ],
        [ "LOGIN", "md__p_r_o_t_o_c_o_l.html#autotoc_md17", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md18", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md19", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md20", null ]
        ] ],
        [ "LIST-USERS", "md__p_r_o_t_o_c_o_l.html#autotoc_md21", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md22", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md23", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md24", null ]
        ] ],
        [ "SEND-DM.", "md__p_r_o_t_o_c_o_l.html#autotoc_md25", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md26", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md27", null ],
          [ "Client side", "md__p_r_o_t_o_c_o_l.html#autotoc_md28", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md29", null ]
        ] ],
        [ "BROADCAST", "md__p_r_o_t_o_c_o_l.html#autotoc_md30", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md31", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md32", null ],
          [ "Client side", "md__p_r_o_t_o_c_o_l.html#autotoc_md33", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md34", null ]
        ] ]
      ] ],
      [ "Additional features", "md__p_r_o_t_o_c_o_l.html#autotoc_md35", [
        [ "CREATE-ROOM.", "md__p_r_o_t_o_c_o_l.html#autotoc_md36", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md37", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md38", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md39", null ]
        ] ],
        [ "LIST-ROOMS", "md__p_r_o_t_o_c_o_l.html#autotoc_md40", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md41", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md42", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md43", null ]
        ] ],
        [ "JOIN-ROOM", "md__p_r_o_t_o_c_o_l.html#autotoc_md44", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md45", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md46", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md47", null ]
        ] ],
        [ "LEAVE-ROOM", "md__p_r_o_t_o_c_o_l.html#autotoc_md48", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md49", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md50", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md51", null ]
        ] ],
        [ "SEND-ROOM", "md__p_r_o_t_o_c_o_l.html#autotoc_md52", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md53", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md54", null ],
          [ "Client side", "md__p_r_o_t_o_c_o_l.html#autotoc_md55", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md56", null ]
        ] ],
        [ "DELETE-ROOM", "md__p_r_o_t_o_c_o_l.html#autotoc_md57", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md58", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md59", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md60", null ]
        ] ],
        [ "PROFILE", "md__p_r_o_t_o_c_o_l.html#autotoc_md61", [
          [ "Parameters", "md__p_r_o_t_o_c_o_l.html#autotoc_md62", null ],
          [ "Error codes", "md__p_r_o_t_o_c_o_l.html#autotoc_md63", null ],
          [ "Examples", "md__p_r_o_t_o_c_o_l.html#autotoc_md64", null ]
        ] ]
      ] ]
    ] ],
    [ "OPIChat", "md__r_e_a_d_m_e.html", [
      [ "Installation and compilation", "md__r_e_a_d_m_e.html#autotoc_md66", null ],
      [ "Usage", "md__r_e_a_d_m_e.html#autotoc_md67", [
        [ "Server", "md__r_e_a_d_m_e.html#autotoc_md68", [
          [ "Actions to perform", "md__r_e_a_d_m_e.html#autotoc_md69", null ]
        ] ],
        [ "Client", "md__r_e_a_d_m_e.html#autotoc_md70", [
          [ "Actions to perform", "md__r_e_a_d_m_e.html#autotoc_md71", null ]
        ] ],
        [ "More information", "md__r_e_a_d_m_e.html#autotoc_md72", null ]
      ] ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ]
      ] ]
    ] ],
    [ "Data Structures", "annotated.html", [
      [ "Data Structures", "annotated.html", "annotated_dup" ],
      [ "Data Structure Index", "classes.html", null ],
      [ "Data Fields", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Variables", "functions_vars.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "Globals", "globals.html", [
        [ "All", "globals.html", "globals_dup" ],
        [ "Functions", "globals_func.html", "globals_func" ],
        [ "Variables", "globals_vars.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Enumerator", "globals_eval.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"annotated.html",
"init__socket_8h.html#add99ba4ea70b8f66170823cad9a55fa4",
"safe__io_8h.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';