#Note: In order to use the vlc module in python, the user system should have vlc media player already installed on the machine.
#pip install python-vlc

import vlc

# Replace 'rtsp://your_stream_url' with your actual RTSP stream URL
stream_url = 'rtsp://192.168.1.1'

# Creating VLC instance
vlc_instance = vlc.Instance('--no-xlib')

# Creating VLC media player
player = vlc_instance.media_player_new()

# Toggling full screen
#player.toggle_fullscreen()

# Creating media
media = vlc_instance.media_new(stream_url)

# Setting media to the player
player.set_media(media)

# Playing the media
player.play()

# Wait for player to finish playing
while player.get_state() != vlc.State.Ended:
    pass

# Release the player and media objects
media.release()
player.release()