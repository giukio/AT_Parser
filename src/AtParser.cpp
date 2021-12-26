/*!
 *  @file AtParser.cpp
 *
 *  BSD 3-Clause License
 *  Copyright (c) 2021, Giulio Berti
 *  All rights reserved.
 *  
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *  
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  
 *  3. Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *  
 */

#include <AtParser.h>

AtParser::AtParser()
{
    AtParser::AddCommand(new AtCommand("version", [](std::vector<String> params){Serial.println("Version 1.0");}) );
}

AtParser::~AtParser()
{
}

void AtParser::AddCommand(AtCommand *cmd)
{
    _commands.push_back(cmd);
}


int8_t AtParser::Parse(String command)
{
    Serial.print(command);
    command.toLowerCase();

    if(command == "at\r\n")
    {
        Serial.println("OK");
        return 0;
    }

    int16_t nameIndex = command.indexOf("at+");
    if(nameIndex != -1){
        int16_t paramIndex = command.indexOf(":");
        String name;
        if(paramIndex < 0)
        {
            name = command.substring(nameIndex+3, command.lastIndexOf("\r\n"));
        }
        else
        {
            name = command.substring(nameIndex+3, paramIndex);
        }
        // Serial.printf("Function name:%s\n",name.c_str());

        String params = command;
        std::vector<String> paramsVec;
        while(paramIndex > -1)
        {
            params = params.substring(paramIndex+1);
            paramIndex = params.indexOf(":");
            String param = params.substring(0,paramIndex);
            paramsVec.push_back(param);
            // Serial.printf("Argument %d: %s\n", paramsVec.size(), param.c_str());
        }

        for(AtCommand* cmd : _commands)
        {
            if((cmd)->GetName() == name)
            {
                (cmd)->Execute(paramsVec);
                return 0;
            }
        }

        return -1;
    }
    
    return -1;
}



AtCommand::AtCommand(String name, void (*callback)(std::vector<String> params))
{
    _name = name;
    _callback = callback;
}

AtCommand::~AtCommand()
{
}

void AtCommand::Execute(std::vector<String> params)
{
    _callback(params);
}

String AtCommand::GetName()
{
    return _name;
}
