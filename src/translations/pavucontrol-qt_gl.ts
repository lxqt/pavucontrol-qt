<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="gl">
<context>
    <name>CardWidget</name>
    <message>
        <location filename="../cardwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="29"/>
        <source>Card Name</source>
        <translation>Nome da tarxeta</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="47"/>
        <source>Profile:</source>
        <translation>Perfil:</translation>
    </message>
    <message>
        <location filename="../cardwidget.cc" line="67"/>
        <source>pa_context_set_card_profile_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_card_profile_by_index()</translation>
    </message>
</context>
<context>
    <name>Channel</name>
    <message>
        <location filename="../channel.cc" line="89"/>
        <source>%1% (%2dB)</source>
        <comment>volume slider label [X% (YdB)]</comment>
        <translation></translation>
    </message>
    <message>
        <location filename="../channel.cc" line="93"/>
        <source>%1%</source>
        <comment>volume slider label [X%]</comment>
        <translation></translation>
    </message>
    <message>
        <location filename="../channel.cc" line="160"/>
        <source>&lt;small&gt;Silence&lt;/small&gt;</source>
        <translation>&lt;small&gt;Silencio&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="160"/>
        <source>&lt;small&gt;Min&lt;/small&gt;</source>
        <translation>&lt;small&gt;Mín&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="162"/>
        <source>&lt;small&gt;100% (0dB)&lt;/small&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../channel.cc" line="165"/>
        <source>&lt;small&gt;&lt;i&gt;Base&lt;/i&gt;&lt;/small&gt;</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>ChannelWidget</name>
    <message>
        <location filename="../channelwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../channelwidget.ui" line="20"/>
        <source>&lt;b&gt;left-front&lt;/b&gt;</source>
        <translation>&lt;b&gt;frontal-esquerdo&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../channelwidget.ui" line="34"/>
        <source>&lt;small&gt;50%&lt;/small&gt;</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DeviceWidget</name>
    <message>
        <location filename="../devicewidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="36"/>
        <source>Device Title</source>
        <translation>Título do dispositivo</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="56"/>
        <source>Mute audio</source>
        <translation>Silenciar o son</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="69"/>
        <source>Lock channels together</source>
        <translation>Bloquear as canles xuntas</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="85"/>
        <source>Set as fallback</source>
        <translation>Estabelecer como alternativa</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="103"/>
        <source>&lt;b&gt;Port:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Porto:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="126"/>
        <source>Show advanced options</source>
        <translation>Amosar as opcións avanzadas</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="172"/>
        <source>PCM</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="182"/>
        <source>AC3</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="189"/>
        <source>EAC3</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="196"/>
        <source>DTS</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="203"/>
        <source>MPEG</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="210"/>
        <source>AAC</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="238"/>
        <source>&lt;b&gt;Latency offset:&lt;/b&gt;</source>
        <translation>&lt;b&gt;Desviación da latencia:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="245"/>
        <source> ms</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="42"/>
        <source>Rename device...</source>
        <translation>Renomear o dispositivo...</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="156"/>
        <source>pa_context_set_port_latency_offset() failed</source>
        <translation>Produciuse un fallo de pa_context_set_port_latency_offset()</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="227"/>
        <source>Sorry, but device renaming is not supported.</source>
        <translation>Sentímolo, non é posíbel renomear o dispositivo.</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="228"/>
        <source>You need to load module-device-manager in the PulseAudio server in order to rename devices</source>
        <translation>É necesario cargar «module-device-manager» no servidor de PulseAudio para poder renomear dispositivos</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="234"/>
        <source>Rename device %1 to:</source>
        <translation>Renomear o dispositivo %1 como:</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="241"/>
        <source>pa_ext_device_manager_set_device_description() failed</source>
        <translation>Produciuse un fallo de pa_ext_device_manager_set_device_description()</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Volume Control</source>
        <translation>Control do volume</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="55"/>
        <source>&lt;i&gt;No application is currently playing audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;Non hai ningunha aplicación reproducindo son.&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="66"/>
        <location filename="../mainwindow.ui" line="131"/>
        <location filename="../mainwindow.ui" line="196"/>
        <location filename="../mainwindow.ui" line="261"/>
        <source>Show:</source>
        <translation>Amosar:</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="74"/>
        <location filename="../mainwindow.ui" line="139"/>
        <source>All Streams</source>
        <translation>Todos os fluxos</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="79"/>
        <location filename="../mainwindow.ui" line="144"/>
        <source>Applications</source>
        <translation>Aplicacións</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="84"/>
        <location filename="../mainwindow.ui" line="149"/>
        <source>Virtual Streams</source>
        <translation>Fluxos virtuais</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="120"/>
        <source>&lt;i&gt;No application is currently recording audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;Non hai ningunha aplicación gravando son.&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="185"/>
        <source>&lt;i&gt;No output devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;Non hai dispositivos de saída dispoñíbeis&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="204"/>
        <source>All Output Devices</source>
        <translation>Todos os dispositivos de saída</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="209"/>
        <source>Hardware Output Devices</source>
        <translation>Dispositivos de saída por hardware</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="214"/>
        <source>Virtual Output Devices</source>
        <translation>Dispositivos de saída virtuais</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="28"/>
        <source>&amp;Playback</source>
        <translation>&amp;Reprodución</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="93"/>
        <source>&amp;Recording</source>
        <translation>&amp;Gravación</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="158"/>
        <source>&amp;Output Devices</source>
        <translation>Dispositivos de &amp;saída</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="223"/>
        <source>&amp;Input Devices</source>
        <translation>Dispositivos de &amp;entrada</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="250"/>
        <source>&lt;i&gt;No input devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;Non hai dispositivos de entrada dispoñíbeis&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="269"/>
        <source>All Input Devices</source>
        <translation>Todos os dispositivos de entrada</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="274"/>
        <source>All Except Monitors</source>
        <translation>Todos, agás os monitores</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="279"/>
        <source>Hardware Input Devices</source>
        <translation>Dispositivos de entrada por hardware</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="284"/>
        <source>Virtual Input Devices</source>
        <translation>Dispositivos de entrada virtuais</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="289"/>
        <source>Monitors</source>
        <translation>Monitores</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="298"/>
        <source>&amp;Configuration</source>
        <translation>&amp;Configuración</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="325"/>
        <source>&lt;i&gt;No cards available for configuration&lt;/i&gt;</source>
        <translation>&lt;i&gt;Non hai tarxetas dispoñíbeis para configurar&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="336"/>
        <source>Show volume meters</source>
        <translation>Amosar os medidores de volume</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="347"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="159"/>
        <source> (plugged in)</source>
        <translation> (conectado)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="163"/>
        <location filename="../mainwindow.cc" line="259"/>
        <source> (unavailable)</source>
        <translation> (non dispoñíbel)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="165"/>
        <location filename="../mainwindow.cc" line="256"/>
        <source> (unplugged)</source>
        <translation> (desconectado)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="390"/>
        <source>Failed to read data from stream</source>
        <translation>Produciuse un fallo ao ler datos do fluxo</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="434"/>
        <source>Peak detect</source>
        <translation>Detección de picos</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="435"/>
        <source>Failed to create monitoring stream</source>
        <translation>Produciuse un fallo ao crear o fluxo de monitorización</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="450"/>
        <source>Failed to connect monitoring stream</source>
        <translation>Produciuse un fallo ao conectarse co fluxo de monitorización</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="590"/>
        <source>Ignoring sink-input due to it being designated as an event and thus handled by the Event widget</source>
        <translation>Ignorarase o colector de entrada por ter sido designado como un evento, sendo polo tanto administrado polo trebello de eventos</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="762"/>
        <source>System Sounds</source>
        <translation>Sons do sistema</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="1092"/>
        <source>Establishing connection to PulseAudio. Please wait...</source>
        <translation>Estabelecendo a conexión co PulseAudio. Agarde...</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../pavucontrol.cc" line="66"/>
        <source>Error</source>
        <translation>Erro</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="87"/>
        <source>Card callback failure</source>
        <translation>Produciuse un fallo na chamada de resposta da tarxeta</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="110"/>
        <source>Sink callback failure</source>
        <translation>Produciuse un fallo na chamada de resposta do colector</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="133"/>
        <source>Source callback failure</source>
        <translation>Produciuse un fallo na chamada de resposta da fonte</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="152"/>
        <source>Sink input callback failure</source>
        <translation>Produciuse un fallo na chamada de entrada do colector</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="171"/>
        <source>Source output callback failure</source>
        <translation>Produciuse un fallo na chamada de resposta da saída da fonte</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="211"/>
        <source>Client callback failure</source>
        <translation>Produciuse un fallo na chamada de resposta do cliente</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="227"/>
        <source>Server info callback failure</source>
        <translation>Produciuse un fallo na chamada de información do servidor</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="245"/>
        <location filename="../pavucontrol.cc" line="542"/>
        <source>Failed to initialize stream_restore extension: %s</source>
        <translation>Produciuse un fallo ao iniciar a extensión «stream_restore»: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="263"/>
        <source>pa_ext_stream_restore_read() failed</source>
        <translation>Produciuse un fallo de pa_ext_stream_restore_read()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="281"/>
        <location filename="../pavucontrol.cc" line="556"/>
        <source>Failed to initialize device restore extension: %s</source>
        <translation>Produciuse un fallo ao iniciar a extensión de restauración do dispositivo: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="302"/>
        <source>pa_ext_device_restore_read_sink_formats() failed</source>
        <translation>Produciuse un fallo de pa_ext_device_restore_read_sink_formats()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="320"/>
        <location filename="../pavucontrol.cc" line="569"/>
        <source>Failed to initialize device manager extension: %s</source>
        <translation>Produciuse un fallo ao iniciar a extensión de xestión do dispositivo: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="339"/>
        <source>pa_ext_device_manager_read() failed</source>
        <translation>Produciuse un fallo de pa_ext_device_manager_read()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="356"/>
        <source>pa_context_get_sink_info_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_get_sink_info_by_index()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="369"/>
        <source>pa_context_get_source_info_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_get_source_info_by_index()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="382"/>
        <location filename="../pavucontrol.cc" line="395"/>
        <source>pa_context_get_sink_input_info() failed</source>
        <translation>Produciuse un fallo de pa_context_get_sink_input_info()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="408"/>
        <source>pa_context_get_client_info() failed</source>
        <translation>Produciuse un fallo de pa_context_get_client_info()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="418"/>
        <location filename="../pavucontrol.cc" line="483"/>
        <source>pa_context_get_server_info() failed</source>
        <translation>Produciuse un fallo de pa_context_get_server_info()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="431"/>
        <source>pa_context_get_card_info_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_get_card_info_by_index()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="474"/>
        <source>pa_context_subscribe() failed</source>
        <translation>Produciuse un fallo de pa_context_subscribe()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="490"/>
        <source>pa_context_client_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_client_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="497"/>
        <source>pa_context_get_card_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_get_card_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="504"/>
        <source>pa_context_get_sink_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_get_sink_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="511"/>
        <source>pa_context_get_source_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_get_source_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="518"/>
        <source>pa_context_get_sink_input_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_get_sink_input_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="525"/>
        <source>pa_context_get_source_output_info_list() failed</source>
        <translation>Produciuse un fallo de pa_context_get_source_output_info_list()</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="584"/>
        <location filename="../pavucontrol.cc" line="635"/>
        <source>Connection failed, attempting reconnect</source>
        <translation>Produciuse un fallo na conexión, tentando volver conectar</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="607"/>
        <location filename="../pavucontrol.cc" line="663"/>
        <source>PulseAudio Volume Control</source>
        <translation>Control de volume do PulseAudio</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="622"/>
        <source>Connection to PulseAudio failed. Automatic retry in 5s.&lt;br&gt;&lt;br&gt;In this case this is likely because PULSE_SERVER in the Environment/X11 Root Window Propertiesor default-server in client.conf is misconfigured.&lt;br&gt;This situation can also arrise when PulseAudio crashed and left stale details in the X11 Root Window.&lt;br&gt;If this is the case, then PulseAudio should autospawn again, or if this is not configured you shouldrun start-pulseaudio-x11 manually.</source>
        <translation>Fallou a conexión a PulseAudio.  Reintento automático en 5 segundos.&lt;br&gt;&lt;br&gt;Neste caso, é probable que PULSE_SERVER no entorno/X11 Propiedades da xanela raíz ou o servidor predeterminado en client.conf está mal configurado.&lt;br&gt;Esta situación tamén pode darse cando PulseAudio falla e sae  detalles obsoletos na xanela raíz de X11.&lt;br&gt;Se este é o caso, PulseAudio debería aparecer automaticamente de novo ou, se isto non está configurado, debería executar start-pulseaudio-x11 manualmente.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="671"/>
        <source>Select a specific tab on load.</source>
        <translation>Seleccionar unha lapela específica ao cargar.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="674"/>
        <source>Retry forever if pa quits (every 5 seconds).</source>
        <translation>Reintentar indefinidamente se PA se pecha (cada 5 segundos).</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="677"/>
        <source>Maximize the window.</source>
        <translation>Maximizar a xanela.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="702"/>
        <source>Fatal Error: Unable to connect to PulseAudio</source>
        <translation>Produciuse un erro grave: Non é posíbel volver conectar co PulseAudio</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../rolewidget.cc" line="59"/>
        <source>pa_ext_stream_restore_write() failed</source>
        <translation>Produciuse un fallo de pa_ext_stream_restore_write()</translation>
    </message>
</context>
<context>
    <name>SinkInputWidget</name>
    <message>
        <location filename="../sinkinputwidget.cc" line="36"/>
        <source>on</source>
        <translation>acendido</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="39"/>
        <source>Terminate Playback</source>
        <translation>Rematar a reprodución</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="52"/>
        <source>Unknown output</source>
        <translation>Saída descoñecida</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="63"/>
        <source>pa_context_set_sink_input_volume() failed</source>
        <translation>Produciuse un fallo de pa_context_set_sink_input_volume()</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="78"/>
        <source>pa_context_set_sink_input_mute() failed</source>
        <translation>Produciuse un fallo de pa_context_set_sink_input_mute()</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="88"/>
        <source>pa_context_kill_sink_input() failed</source>
        <translation>Produciuse un fallo de pa_context_kill_sink_input()</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="113"/>
        <source>pa_context_move_sink_input_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_move_sink_input_by_index()</translation>
    </message>
</context>
<context>
    <name>SinkWidget</name>
    <message>
        <location filename="../sinkwidget.cc" line="81"/>
        <source>pa_context_set_sink_volume_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_sink_volume_by_index()</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="96"/>
        <source>pa_context_set_sink_mute_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_sink_mute_by_index()</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="110"/>
        <source>pa_context_set_default_sink() failed</source>
        <translation>Produciuse un fallo de pa_context_set_default_sink()</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="126"/>
        <source>pa_context_set_sink_port_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_sink_port_by_index()</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="166"/>
        <source>pa_ext_device_restore_save_sink_formats() failed</source>
        <translation>Produciuse un fallo de pa_ext_device_restore_save_sink_formats()</translation>
    </message>
</context>
<context>
    <name>SourceOutputWidget</name>
    <message>
        <location filename="../sourceoutputwidget.cc" line="35"/>
        <source>from</source>
        <translation>dende</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="39"/>
        <source>Terminate Recording</source>
        <translation>Rematar a gravación</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="59"/>
        <source>Unknown input</source>
        <translation>Entrada descoñecida</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="71"/>
        <source>pa_context_set_source_output_volume() failed</source>
        <translation>Produciuse un fallo de pa_context_set_source_output_volume()</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="86"/>
        <source>pa_context_set_source_output_mute() failed</source>
        <translation>Produciuse un fallo de pa_context_set_source_output_mute()</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="97"/>
        <source>pa_context_kill_source_output() failed</source>
        <translation>Produciuse un fallo de pa_context_kill_source_output()</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="124"/>
        <source>pa_context_move_source_output_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_move_source_output_by_index()</translation>
    </message>
</context>
<context>
    <name>SourceWidget</name>
    <message>
        <location filename="../sourcewidget.cc" line="35"/>
        <source>pa_context_set_source_volume_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_source_volume_by_index()</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="50"/>
        <source>pa_context_set_source_mute_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_source_mute_by_index()</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="64"/>
        <source>pa_context_set_default_source() failed</source>
        <translation>Produciuse un fallo de pa_context_set_default_source()</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="80"/>
        <source>pa_context_set_source_port_by_index() failed</source>
        <translation>Produciuse un fallo de pa_context_set_source_port_by_index()</translation>
    </message>
</context>
<context>
    <name>StreamWidget</name>
    <message>
        <location filename="../streamwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="32"/>
        <source>Device Title</source>
        <translation>Título do dispositivo</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="52"/>
        <source>direction</source>
        <translation>dirección</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="59"/>
        <source>device</source>
        <translation>dispositivo</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="66"/>
        <source>Mute audio</source>
        <translation>Silenciar o son</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="79"/>
        <source>Lock channels together</source>
        <translation>Bloquear as canles xuntas</translation>
    </message>
    <message>
        <location filename="../streamwidget.cc" line="34"/>
        <source>Terminate</source>
        <translation>Rematar</translation>
    </message>
</context>
</TS>
