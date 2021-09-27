<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ko">
<context>
    <name>CardWidget</name>
    <message>
        <location filename="../cardwidget.ui" line="14"/>
        <source>Form</source>
        <translation>형태</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="29"/>
        <source>Card Name</source>
        <translation>카드 이름</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="47"/>
        <source>Profile:</source>
        <translation>프로필:</translation>
    </message>
    <message>
        <location filename="../cardwidget.cc" line="67"/>
        <source>pa_context_set_card_profile_by_index() failed</source>
        <translation>pa_context_set_card_profile_by_index() 실패함</translation>
    </message>
</context>
<context>
    <name>Channel</name>
    <message>
        <location filename="../channel.cc" line="89"/>
        <source>%1% (%2dB)</source>
        <comment>volume slider label [X% (YdB)]</comment>
        <translation>%1% (%2dB)</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="93"/>
        <source>%1%</source>
        <comment>volume slider label [X%]</comment>
        <translation>%1%</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="160"/>
        <source>&lt;small&gt;Silence&lt;/small&gt;</source>
        <translation>&lt;small&gt;무음&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="160"/>
        <source>&lt;small&gt;Min&lt;/small&gt;</source>
        <translation>&lt;small&gt;최소&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="162"/>
        <source>&lt;small&gt;100% (0dB)&lt;/small&gt;</source>
        <translation>&lt;small&gt;100% (0dB)&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="165"/>
        <source>&lt;small&gt;&lt;i&gt;Base&lt;/i&gt;&lt;/small&gt;</source>
        <translation>&lt;small&gt;&lt;i&gt;기본&lt;/i&gt;&lt;/small&gt;</translation>
    </message>
</context>
<context>
    <name>ChannelWidget</name>
    <message>
        <location filename="../channelwidget.ui" line="14"/>
        <source>Form</source>
        <translation>형태</translation>
    </message>
    <message>
        <location filename="../channelwidget.ui" line="20"/>
        <source>&lt;b&gt;left-front&lt;/b&gt;</source>
        <translation>&lt;b&gt;왼쪽 앞&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../channelwidget.ui" line="34"/>
        <source>&lt;small&gt;50%&lt;/small&gt;</source>
        <translation>&lt;small&gt;50%&lt;/small&gt;</translation>
    </message>
</context>
<context>
    <name>DeviceWidget</name>
    <message>
        <location filename="../devicewidget.ui" line="14"/>
        <source>Form</source>
        <translation>형태</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="36"/>
        <source>Device Title</source>
        <translation>장치 제목</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="56"/>
        <source>Mute audio</source>
        <translation>오디오 음소거</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="69"/>
        <source>Lock channels together</source>
        <translation>사운드 채널을 동시에  잠금</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="85"/>
        <source>Set as fallback</source>
        <translation>대체로 설정</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="103"/>
        <source>&lt;b&gt;Port:&lt;/b&gt;</source>
        <translation>&lt;b&gt;포트:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="126"/>
        <source>Show advanced options</source>
        <translation>고급 옵션 표시</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="172"/>
        <source>PCM</source>
        <translation>PCM</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="182"/>
        <source>AC3</source>
        <translation>AC3</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="189"/>
        <source>EAC3</source>
        <translation>EAC3</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="196"/>
        <source>DTS</source>
        <translation>DTS</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="203"/>
        <source>MPEG</source>
        <translation>MPEG</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="210"/>
        <source>AAC</source>
        <translation>AAC</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="238"/>
        <source>&lt;b&gt;Latency offset:&lt;/b&gt;</source>
        <translation>&lt;b&gt;대기시간 오프셋:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="245"/>
        <source> ms</source>
        <translation> ms</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="41"/>
        <source>Rename device...</source>
        <translation>장치 이름변경...</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="155"/>
        <source>pa_context_set_port_latency_offset() failed</source>
        <translation>pa_context_set_port_latency_offset() 실패함</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="226"/>
        <source>Sorry, but device renaming is not supported.</source>
        <translation>죄송합니다. 장치 이름변경은 지원되지 않습니다.</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="227"/>
        <source>You need to load module-device-manager in the PulseAudio server in order to rename devices</source>
        <translation>장치 이름을 바꾸려면 PulseAudio 서버에서 module-device-manager를 불러와야 합니다</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="233"/>
        <source>Rename device %1 to:</source>
        <translation>다음으로 %1 장치 이름변경:</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="240"/>
        <source>pa_ext_device_manager_set_device_description() failed</source>
        <translation>pa_ext_device_manager_set_device_description() 실패함</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Volume Control</source>
        <translation>음량 조절</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="55"/>
        <source>&lt;i&gt;No application is currently playing audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;현재 오디오를 재생중인 응용 프로그램이 없습니다.&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="66"/>
        <location filename="../mainwindow.ui" line="131"/>
        <location filename="../mainwindow.ui" line="196"/>
        <location filename="../mainwindow.ui" line="261"/>
        <source>Show:</source>
        <translation>표시:</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="74"/>
        <location filename="../mainwindow.ui" line="139"/>
        <source>All Streams</source>
        <translation>모든 스트림</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="79"/>
        <location filename="../mainwindow.ui" line="144"/>
        <source>Applications</source>
        <translation>응용프로그램</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="84"/>
        <location filename="../mainwindow.ui" line="149"/>
        <source>Virtual Streams</source>
        <translation>가상 스트림</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="120"/>
        <source>&lt;i&gt;No application is currently recording audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;현재 오디오를 녹음중인 응용프로그램이 없습니다.&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="185"/>
        <source>&lt;i&gt;No output devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;사용 가능한 출력 장치 없음&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="204"/>
        <source>All Output Devices</source>
        <translation>모든 출력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="209"/>
        <source>Hardware Output Devices</source>
        <translation>하드웨어 출력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="214"/>
        <source>Virtual Output Devices</source>
        <translation>가상 출력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="28"/>
        <source>&amp;Playback</source>
        <translation>재생(&amp;P)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="93"/>
        <source>&amp;Recording</source>
        <translation>녹음(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="158"/>
        <source>&amp;Output Devices</source>
        <translation>출력 장치(&amp;O)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="223"/>
        <source>&amp;Input Devices</source>
        <translation>입력 장치(&amp;I)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="250"/>
        <source>&lt;i&gt;No input devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;사용 가능한 입력 장치 없음&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="269"/>
        <source>All Input Devices</source>
        <translation>모든 입력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="274"/>
        <source>All Except Monitors</source>
        <translation>모니터를 제외한 모든 항목</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="279"/>
        <source>Hardware Input Devices</source>
        <translation>하드웨어 입력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="284"/>
        <source>Virtual Input Devices</source>
        <translation>가상 입력 장치</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="289"/>
        <source>Monitors</source>
        <translation>모니터</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="298"/>
        <source>&amp;Configuration</source>
        <translation>구성(&amp;C)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="325"/>
        <source>&lt;i&gt;No cards available for configuration&lt;/i&gt;</source>
        <translation>&lt;i&gt;구성에 사용할 수 있는 카드 없음&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="336"/>
        <source>Show volume meters</source>
        <translation>음량 미터 표시</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="347"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="159"/>
        <source> (plugged in)</source>
        <translation> (연결됨)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="163"/>
        <location filename="../mainwindow.cc" line="257"/>
        <source> (unavailable)</source>
        <translation> (사용할 수 없음)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="165"/>
        <location filename="../mainwindow.cc" line="254"/>
        <source> (unplugged)</source>
        <translation> (분리됨)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="388"/>
        <source>Failed to read data from stream</source>
        <translation>스트림에서 데이터를 읽지 못했습니다</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="432"/>
        <source>Peak detect</source>
        <translation>피크 감지</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="433"/>
        <source>Failed to create monitoring stream</source>
        <translation>모니터링 스트림을 만들지 못했습니다</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="448"/>
        <source>Failed to connect monitoring stream</source>
        <translation>모니터링 스트림을 연결하지 못했습니다</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="587"/>
        <source>Ignoring sink-input due to it being designated as an event and thus handled by the Event widget</source>
        <translation>이벤트로 지정되어 이벤트 위젯에서 처리되는 싱크 입력 무시하는 중</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="759"/>
        <source>System Sounds</source>
        <translation>시스템 사운드</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="1089"/>
        <source>Establishing connection to PulseAudio. Please wait...</source>
        <translation>PulseAudio에 연결하는 중입니다. 기다려 주세요...</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../pavucontrol.cc" line="66"/>
        <source>Error</source>
        <translation>오류</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="87"/>
        <source>Card callback failure</source>
        <translation>카드 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="110"/>
        <source>Sink callback failure</source>
        <translation>싱크 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="133"/>
        <source>Source callback failure</source>
        <translation>소스 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="152"/>
        <source>Sink input callback failure</source>
        <translation>싱크 입력 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="171"/>
        <source>Source output callback failure</source>
        <translation>소스 출력 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="211"/>
        <source>Client callback failure</source>
        <translation>클라이언트 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="227"/>
        <source>Server info callback failure</source>
        <translation>서버 정보 콜백 실패</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="245"/>
        <location filename="../pavucontrol.cc" line="542"/>
        <source>Failed to initialize stream_restore extension: %s</source>
        <translation>stream_restore 확장을 초기화하지 못했습니다: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="263"/>
        <source>pa_ext_stream_restore_read() failed</source>
        <translation>pa_ext_stream_restore_read() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="281"/>
        <location filename="../pavucontrol.cc" line="556"/>
        <source>Failed to initialize device restore extension: %s</source>
        <translation>장치 복원 확장을 초기화하지 못했습니다: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="302"/>
        <source>pa_ext_device_restore_read_sink_formats() failed</source>
        <translation>pa_ext_device_restore_read_sink_formats() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="320"/>
        <location filename="../pavucontrol.cc" line="569"/>
        <source>Failed to initialize device manager extension: %s</source>
        <translation>장치 관리자 확장을 초기화하지 못했습니다: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="339"/>
        <source>pa_ext_device_manager_read() failed</source>
        <translation>pa_ext_device_manager_read() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="356"/>
        <source>pa_context_get_sink_info_by_index() failed</source>
        <translation>pa_context_get_sink_info_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="369"/>
        <source>pa_context_get_source_info_by_index() failed</source>
        <translation>pa_context_get_source_info_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="382"/>
        <location filename="../pavucontrol.cc" line="395"/>
        <source>pa_context_get_sink_input_info() failed</source>
        <translation>pa_context_get_sink_input_info() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="408"/>
        <source>pa_context_get_client_info() failed</source>
        <translation>pa_context_get_client_info() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="418"/>
        <location filename="../pavucontrol.cc" line="483"/>
        <source>pa_context_get_server_info() failed</source>
        <translation>pa_context_get_server_info() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="431"/>
        <source>pa_context_get_card_info_by_index() failed</source>
        <translation>pa_context_get_card_info_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="474"/>
        <source>pa_context_subscribe() failed</source>
        <translation>pa_context_subscribe() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="490"/>
        <source>pa_context_client_info_list() failed</source>
        <translation>pa_context_client_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="497"/>
        <source>pa_context_get_card_info_list() failed</source>
        <translation>pa_context_get_card_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="504"/>
        <source>pa_context_get_sink_info_list() failed</source>
        <translation>pa_context_get_sink_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="511"/>
        <source>pa_context_get_source_info_list() failed</source>
        <translation>pa_context_get_source_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="518"/>
        <source>pa_context_get_sink_input_info_list() failed</source>
        <translation>pa_context_get_sink_input_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="525"/>
        <source>pa_context_get_source_output_info_list() failed</source>
        <translation>pa_context_get_source_output_info_list() 실패함</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="584"/>
        <location filename="../pavucontrol.cc" line="635"/>
        <source>Connection failed, attempting reconnect</source>
        <translation>연결 실패, 다시 연결 시도</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="607"/>
        <location filename="../pavucontrol.cc" line="664"/>
        <source>PulseAudio Volume Control</source>
        <translation>PulseAudio 음량 조절</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="622"/>
        <source>Connection to PulseAudio failed. Automatic retry in 5s

In this case this is likely because PULSE_SERVER in the Environment/X11 Root Window Properties
or default-server in client.conf is misconfigured.
This situation can also arrise when PulseAudio crashed and left stale details in the X11 Root Window.
If this is the case, then PulseAudio should autospawn again, or if this is not configured you should
run start-pulseaudio-x11 manually.</source>
        <translation>PulseAudio에 연결하지 못했습니다. 5초 후 자동 재시도

이 경우 Environment/X11 루트 창 속성의 PULSE_SERVER 또는 client.conf의 default-server가 
잘못 구성되었기 때문일 수 있습니다.
이 상황은 PulseAudio가 충돌하고 X11 루트 창에 오래된 세부 정보를 남길 때도 발생할 수 있습니다.
이 경우 PulseAudio가 다시 자동 생성되거나 구성되지 않은 경우 start-pulseaudio-x11을 수동으로 
실행해야 합니다.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="672"/>
        <source>Select a specific tab on load.</source>
        <translation>불러오기 시 특정 탭을 선택합니다.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="675"/>
        <source>Retry forever if pa quits (every 5 seconds).</source>
        <translation>pa가 종료되면 영원히 재시도합니다(5초마다).</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="678"/>
        <source>Maximize the window.</source>
        <translation>창을 최대화합니다.</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="703"/>
        <source>Fatal Error: Unable to connect to PulseAudio</source>
        <translation>치명적인 오류: PulseAudio에 연결할 수 없습니다</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../rolewidget.cc" line="59"/>
        <source>pa_ext_stream_restore_write() failed</source>
        <translation>pa_ext_stream_restore_write() 실패함</translation>
    </message>
</context>
<context>
    <name>SinkInputWidget</name>
    <message>
        <location filename="../sinkinputwidget.cc" line="36"/>
        <source>on</source>
        <translation>켜기</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="39"/>
        <source>Terminate Playback</source>
        <translation>재생 종료</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="53"/>
        <source>Unknown output</source>
        <translation>알 수 없는 출력</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="64"/>
        <source>pa_context_set_sink_input_volume() failed</source>
        <translation>pa_context_set_sink_input_volume() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="79"/>
        <source>pa_context_set_sink_input_mute() failed</source>
        <translation>pa_context_set_sink_input_mute() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="89"/>
        <source>pa_context_kill_sink_input() failed</source>
        <translation>pa_context_kill_sink_input() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="114"/>
        <source>pa_context_move_sink_input_by_index() failed</source>
        <translation>pa_context_move_sink_input_by_index() 실패함</translation>
    </message>
</context>
<context>
    <name>SinkWidget</name>
    <message>
        <location filename="../sinkwidget.cc" line="81"/>
        <source>pa_context_set_sink_volume_by_index() failed</source>
        <translation>pa_context_set_sink_volume_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="96"/>
        <source>pa_context_set_sink_mute_by_index() failed</source>
        <translation>pa_context_set_sink_mute_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="110"/>
        <source>pa_context_set_default_sink() failed</source>
        <translation>pa_context_set_default_sink() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="126"/>
        <source>pa_context_set_sink_port_by_index() failed</source>
        <translation>pa_context_set_sink_port_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="166"/>
        <source>pa_ext_device_restore_save_sink_formats() failed</source>
        <translation>pa_ext_device_restore_save_sink_formats() 실패함</translation>
    </message>
</context>
<context>
    <name>SourceOutputWidget</name>
    <message>
        <location filename="../sourceoutputwidget.cc" line="35"/>
        <source>from</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="39"/>
        <source>Terminate Recording</source>
        <translation>녹음 종료</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="60"/>
        <source>Unknown input</source>
        <translation>알 수 없는 입력</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="72"/>
        <source>pa_context_set_source_output_volume() failed</source>
        <translation>pa_context_set_source_output_volume() 실패함</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="87"/>
        <source>pa_context_set_source_output_mute() failed</source>
        <translation>pa_context_set_source_output_mute() 실패함</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="98"/>
        <source>pa_context_kill_source_output() failed</source>
        <translation>pa_context_kill_source_output() 실패함</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="125"/>
        <source>pa_context_move_source_output_by_index() failed</source>
        <translation>pa_context_move_source_output_by_index() 실패함</translation>
    </message>
</context>
<context>
    <name>SourceWidget</name>
    <message>
        <location filename="../sourcewidget.cc" line="35"/>
        <source>pa_context_set_source_volume_by_index() failed</source>
        <translation>pa_context_set_source_volume_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="50"/>
        <source>pa_context_set_source_mute_by_index() failed</source>
        <translation>pa_context_set_source_mute_by_index() 실패함</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="64"/>
        <source>pa_context_set_default_source() failed</source>
        <translation>pa_context_set_default_source() 실패함</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="80"/>
        <source>pa_context_set_source_port_by_index() failed</source>
        <translation>pa_context_set_source_port_by_index() 실패함</translation>
    </message>
</context>
<context>
    <name>StreamWidget</name>
    <message>
        <location filename="../streamwidget.ui" line="14"/>
        <source>Form</source>
        <translation>형태</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="32"/>
        <source>Device Title</source>
        <translation>장치 제목</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="52"/>
        <source>direction</source>
        <translation>방향</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="59"/>
        <source>device</source>
        <translation>장치</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="66"/>
        <source>Mute audio</source>
        <translation>오디오 음소거</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="79"/>
        <source>Lock channels together</source>
        <translation>사운드 채널을 동시에  잠금</translation>
    </message>
    <message>
        <location filename="../streamwidget.cc" line="34"/>
        <source>Terminate</source>
        <translation>종료</translation>
    </message>
</context>
</TS>
