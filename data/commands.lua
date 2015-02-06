local function set_statusbar_time(buf)
    buf:set_statusbar(os.date("%F %H:%M:%S"))
end

local function move_cursor(dy, dx)
    return function(buf)
        buf:move(dy, dx)
    end
end

-- movement
loony.add_command('h', move_cursor( 0, -1))
loony.add_command('l', move_cursor( 0,  1))
loony.add_command('k', move_cursor(-1,  0))
loony.add_command('j', move_cursor( 1,  0))
loony.add_command('time', set_statusbar_time)
