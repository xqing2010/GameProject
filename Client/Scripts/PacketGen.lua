
local protoname = "PBMessage.proto"
local MsgIDFile = "PBAutoGen\\MessageIDEnum.h"
local MsgHandleFile = "PBAutoGen\\MsgHandleDef.h"
local MsgHandlerRegFile = "PBAutoGen\\MsgHandleReg.h"
local MsgRegisterFile   = "PBAutoGen\\MsgRegister.h"
local MsgName = {}



function readProto(path)
	local count = 1
	local file = io.open(path, "r")
	while true do
		local line = file:read()
		if nil == line then
			break
		end
		local i, j = string.find(line, "//")
		if nil ~= i then
			line = string.sub(line, 1, i)
		end

		if nil ~= line then
			local i, j = string.find(line, "message CS")
			local m, n = string.find(line, "message SC")

			if nil ~= i or nil ~= m then
				i, j, msg = string.find(line, "message ([%w_]+)")
				if nil ~= msg then
					MsgName[count] = msg
					count = count + 1
					print(msg .. " " .. count)
				end
			end
		end
	end
	io.close(file)
end

function genMsgID(path)
	local file = io.open(path, "w")

	local num = table.getn(MsgName)
	for i = 1, num do
		file:write("\tMsg_" .. MsgName[i] .. ",\n")
	end

	io.close(file)
end


function writeMsgHandlerContent(file, name)

	file:write("class " .. name .. "Handler : public MessageHandler {\n")
	file:write("public:\n")
	file:write("\tbool Handler(Session &session, Message *msg);\n")
	file:write("};\n")

end

function genMsgHandler(path)

	local file = io.open(path, "w")
	local num = table.getn(MsgName)
	for i = 1, num do
		local msgName= MsgName[i]
		writeMsgHandlerContent(file, msgName)
	end
	io.close(file)

end

function writeMsgHandlerRegister(file, name)
	file:write("\tm_oHandlers[MsgID::Msg_" .. name .. "] = new " .. name .. "Handler();\n")
end


function genMsgHandlerRegister(path)

	local file = io.open(path, "w")
	local num = table.getn(MsgName)
	for i = 1, num do
		local msgName= MsgName[i]
		writeMsgHandlerRegister(file, msgName)
	end
	io.close(file)

end


function writeMsgRegister(file, name)
	file:write("\m_oMsgArray[MsgID::Msg_" .. name .. "] = new " .. name .. "();\n")
end

function genMsgRegister(path)

	local file = io.open(path, "w")
	local num = table.getn(MsgName)
	for i = 1, num do
		local msgName= MsgName[i]
		writeMsgRegister(file, msgName)
	end
	io.close(file)

end


readProto(protoname)
genMsgID(MsgIDFile)
genMsgHandler(MsgHandleFile)
genMsgHandlerRegister(MsgHandlerRegFile)
genMsgRegister(MsgRegisterFile)
