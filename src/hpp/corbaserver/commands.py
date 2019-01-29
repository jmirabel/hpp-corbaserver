#!/usr/bin/env python
# Copyright (c) 2019 CNRS
# Author: Joseph Mirabel
#
# This file is part of hpp-corbaserver.
# hpp-corbaserver is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-corbaserver is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

class _Command(object):
    def __init__ (self, commandName, keep_any=False, client = None):
        from hpp.corbaserver.client import Client
        if client is None: self.client = Client ()
        else             : self.client = client
        self.commandName = commandName
        self.keep_any = keep_any

    def __call__ (self, *args):
        from .problem_solver import _convertToCorbaAny
        arguments = [ _convertToCorbaAny (a) for a in args ]
        any = self.client.problem.runCommand (self.commandName, arguments)
        return any if self.keep_any else any.value()

class Commands (object):
    def __init__ (self, client = None):
        from hpp.corbaserver.client import Client
        if client is None: self.client = Client ()
        else             : self.client = client
        self.populate ()

    def populate (self, keep_any=False):
        commands = self.client.problem.getAvailable ("command")
        for cmd in commands:
            setattr(self, cmd, _Command (cmd, keep_any=keep_any, client=self.client))
