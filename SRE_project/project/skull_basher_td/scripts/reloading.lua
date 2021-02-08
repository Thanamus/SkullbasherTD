function reloading(time)
    local turr = getGameObject()
    local arm = turr:getChildByName("Arm")
    local ball
    local animator
    if (arm) then
        animator = getAnimator(arm)
        ball = arm:getChildByName("Ball")
    else
        animator = getAnimator(turr)
    end
    if(animator and animator:getAnimationState() ~= "reload") then
        animator:setAnimationState("reload")
    else
        if (not animator or animator:getAnimationForState("reload"):hasEnded(time)) then
            if(ball) then
                renderer = getModelRenderer(ball)
                renderer.active = true
            end
            setReadyToShoot(true)
            setAimPos(vec3.new(-1, -1, -1))
        end
    end
end