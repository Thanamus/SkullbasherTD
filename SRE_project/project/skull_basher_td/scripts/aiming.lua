function targeting(enemies)
    local new_target
    local new_order
    local turret = getGameObject()
    local t_pos = getTransform(turret):globalPosition()
    for e=1,#enemies do
        local enemy = enemies[e]
        local e_pos = enemy:getPosition()
        local e_order = enemy:getPathfinder():getCurrentPathIndex()
        local in_range = inCircle(vec2.new(e_pos.x, e_pos.y), vec2.new(t_pos.x, t_pos.y), variables["range"]);
        if(in_range and (not new_order or new_order > e_order)) then
            new_target = enemy
            new_order = e_order
        end
    end
    if(new_target) then
        setTarget(new_target:getGameObject())
    end
end