
-- Craftcore
-- Mod boot script

MOD_DIR = "mods/"..arg[1]
package.path = package.path..";"..MOD_DIR.."/?.lua;"..MOD_DIR.."/?"

require("scripts/craftfile")

cm = craftfile()
require("ui")

--for i, b in ipairs(cm.blocks) do
	--require("mods/"..arg[1].."/blocks/"..b)
	--cc.addBlock()
--end

