local function set_statusbar_time(buf)
    buf:set_statusbar(os.date("%F %H:%M:%S"))
end

loony.add_command('time', set_statusbar_time)
