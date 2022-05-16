-- register label functions
registerLabel("start")
registerLabel("goodbye")

-- creates characters, sprites, and other means
me = Character("Me")
sprite = Sprite2D("Assets/Textures/sprite.png", true);
music = Audio("Assets/Audio/music.mp3")

function start()
    sprite:show()
    music:play()
    me "Hello World!"
    music:stop()
    sprite:hide()
    
    -- jump to another registered label
    jumpToLabel("goodbye")
end

function goodbye()
    me "Goodbye World!"
end