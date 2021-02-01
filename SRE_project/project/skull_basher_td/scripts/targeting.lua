function targeting()
    printStuff()
    local radius = 10
    local new_target
    local new_order
    local turret = getGameObject()
    local t_pos = getTransform(turret):globalPosition
    for e in enemies do

        local e_pos = getTransform(e):globalPosition
        local e_order = e:getPathfinder:getCurrentPathIndex
        local in_range = inCircle(vec2.new(e_pos.x, e_pos.y), vec2.new(t_pos.x, t_pos.y), variables["range"]);
        if(sqr_distance < math.pow(radius, 2)) then
            if(e_order == nil or (new_order > e_order)) then
                new_target = e
                new_order = e_order
            end
        end
    end
    setTarget(new_target)
end