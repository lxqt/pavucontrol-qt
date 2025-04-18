<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="ja">
<context>
    <name>CardWidget</name>
    <message>
        <location filename="../cardwidget.ui" line="14"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="29"/>
        <source>Card Name</source>
        <translation>カード名</translation>
    </message>
    <message>
        <location filename="../cardwidget.ui" line="47"/>
        <source>Profile:</source>
        <translation>プロフィール:</translation>
    </message>
    <message>
        <location filename="../cardwidget.cc" line="67"/>
        <source>pa_context_set_card_profile_by_index() failed</source>
        <translation>pa_context_set_card_profile_by_index() 失敗</translation>
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
        <translation>&lt;small&gt;無音&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="160"/>
        <source>&lt;small&gt;Min&lt;/small&gt;</source>
        <translation>&lt;small&gt;最小&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="162"/>
        <source>&lt;small&gt;100% (0dB)&lt;/small&gt;</source>
        <translation>&lt;small&gt;100% (0dB)&lt;/small&gt;</translation>
    </message>
    <message>
        <location filename="../channel.cc" line="165"/>
        <source>&lt;small&gt;&lt;i&gt;Base&lt;/i&gt;&lt;/small&gt;</source>
        <translation>&lt;small&gt;&lt;i&gt;ベース&lt;/i&gt;&lt;/small&gt;</translation>
    </message>
</context>
<context>
    <name>ChannelWidget</name>
    <message>
        <location filename="../channelwidget.ui" line="14"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location filename="../channelwidget.ui" line="20"/>
        <source>&lt;b&gt;left-front&lt;/b&gt;</source>
        <translation>&lt;b&gt;左前方&lt;/b&gt;</translation>
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
        <translation>フォーム</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="36"/>
        <source>Device Title</source>
        <translation>デバイスのタイトル</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="56"/>
        <source>Mute audio</source>
        <translation>音量をミュートします</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="69"/>
        <source>Lock channels together</source>
        <translation>各チャンネルを同時に</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="85"/>
        <source>Set as fallback</source>
        <translation>代替として設定します</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="103"/>
        <source>&lt;b&gt;Port:&lt;/b&gt;</source>
        <translation>&lt;b&gt;ポート:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="126"/>
        <source>Show advanced options</source>
        <translation>詳細オプションを表示する</translation>
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
        <translation>&lt;b&gt;遅延:&lt;/b&gt;</translation>
    </message>
    <message>
        <location filename="../devicewidget.ui" line="245"/>
        <source> ms</source>
        <translation> ミリ秒</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="42"/>
        <source>Rename device...</source>
        <translation>デバイス名の変更...</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="156"/>
        <source>pa_context_set_port_latency_offset() failed</source>
        <translation>pa_context_set_port_latency_offset() 失敗しました</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="227"/>
        <source>Sorry, but device renaming is not supported.</source>
        <translation>申し訳ありませんが、デバイス名の変更をサポートしていません。</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="228"/>
        <source>You need to load module-device-manager in the PulseAudio server in order to rename devices</source>
        <translation>デバイス名を変更するには PulseAudio サーバーに module-device-manager をロードする必要があります</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="234"/>
        <source>Rename device %1 to:</source>
        <translation>デバイス %1 の名前を変更:</translation>
    </message>
    <message>
        <location filename="../devicewidget.cc" line="241"/>
        <source>pa_ext_device_manager_set_device_description() failed</source>
        <translation>pa_ext_device_manager_set_device_description() 失敗しました</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Volume Control</source>
        <translation>PulseAudio 音量調節</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="28"/>
        <source>&amp;Playback</source>
        <translation>再生(&amp;P)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="55"/>
        <source>&lt;i&gt;No application is currently playing audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;現在音声を再生しているアプリケーションがありません。&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="66"/>
        <location filename="../mainwindow.ui" line="131"/>
        <location filename="../mainwindow.ui" line="196"/>
        <location filename="../mainwindow.ui" line="261"/>
        <source>Show:</source>
        <translation>表示:</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="74"/>
        <location filename="../mainwindow.ui" line="139"/>
        <source>All Streams</source>
        <translation>すべてのストリーム</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="79"/>
        <location filename="../mainwindow.ui" line="144"/>
        <source>Applications</source>
        <translation>アプリケーション</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="84"/>
        <location filename="../mainwindow.ui" line="149"/>
        <source>Virtual Streams</source>
        <translation>仮想のストリーム</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="93"/>
        <source>&amp;Recording</source>
        <translation>録音(&amp;R)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="120"/>
        <source>&lt;i&gt;No application is currently recording audio.&lt;/i&gt;</source>
        <translation>&lt;i&gt;現在録音しているアプリケーションがありません。&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="158"/>
        <source>&amp;Output Devices</source>
        <translation>出力デバイス(&amp;O)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="185"/>
        <source>&lt;i&gt;No output devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;出力デバイスがありません&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="204"/>
        <source>All Output Devices</source>
        <translation>すべての出力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="209"/>
        <source>Hardware Output Devices</source>
        <translation>ハードウェアの出力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="214"/>
        <source>Virtual Output Devices</source>
        <translation>仮想の出力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="223"/>
        <source>&amp;Input Devices</source>
        <translation>入力デバイス(&amp;I)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="250"/>
        <source>&lt;i&gt;No input devices available&lt;/i&gt;</source>
        <translation>&lt;i&gt;入力デバイスがありません&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="269"/>
        <source>All Input Devices</source>
        <translation>すべての入力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="274"/>
        <source>All Except Monitors</source>
        <translation>モニター以外のすべて</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="279"/>
        <source>Hardware Input Devices</source>
        <translation>ハードウェアの入力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="284"/>
        <source>Virtual Input Devices</source>
        <translation>仮想の入力デバイス</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="289"/>
        <source>Monitors</source>
        <translation>モニター</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="298"/>
        <source>&amp;Configuration</source>
        <translation>設定(&amp;C)</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="325"/>
        <source>&lt;i&gt;No cards available for configuration&lt;/i&gt;</source>
        <translation>&lt;i&gt;設定するカードがありません&lt;/i&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="336"/>
        <source>Show volume meters</source>
        <translation>音量メーターを表示する</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="347"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="159"/>
        <source> (plugged in)</source>
        <translation> (プラグイン)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="163"/>
        <location filename="../mainwindow.cc" line="259"/>
        <source> (unavailable)</source>
        <translation> (使用不可)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="165"/>
        <location filename="../mainwindow.cc" line="256"/>
        <source> (unplugged)</source>
        <translation> (未接続)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="390"/>
        <source>Failed to read data from stream</source>
        <translation>ストリームからのデータの読込みに失敗</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="434"/>
        <source>Peak detect</source>
        <translation>ピーク検出</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="435"/>
        <source>Failed to create monitoring stream</source>
        <translation>モニタリングストリームの生成に失敗</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="450"/>
        <source>Failed to connect monitoring stream</source>
        <translation>モニタリングストリームへの接続に失敗</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="590"/>
        <source>Ignoring sink-input due to it being designated as an event and thus handled by the Event widget</source>
        <translation>これはイベントとして指定されていてイベントウィジェットによって処理されるため、sink-input を無視します</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="762"/>
        <source>System Sounds</source>
        <translation>システム音</translation>
    </message>
    <message>
        <location filename="../mainwindow.cc" line="1092"/>
        <source>Establishing connection to PulseAudio. Please wait...</source>
        <translation>PulseAudio との接続を確立中です。少々お待ち下さい...</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../pavucontrol.cc" line="66"/>
        <source>Error</source>
        <translation>エラー</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="87"/>
        <source>Card callback failure</source>
        <translation>カード回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="110"/>
        <source>Sink callback failure</source>
        <translation>受信側回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="133"/>
        <source>Source callback failure</source>
        <translation>音源回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="152"/>
        <source>Sink input callback failure</source>
        <translation>受信側入力回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="171"/>
        <source>Source output callback failure</source>
        <translation>音源出力回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="211"/>
        <source>Client callback failure</source>
        <translation>クライアント回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="227"/>
        <source>Server info callback failure</source>
        <translation>サーバー情報回収失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="245"/>
        <location filename="../pavucontrol.cc" line="542"/>
        <source>Failed to initialize stream_restore extension: %s</source>
        <translation>stream_restore 拡張の初期化に失敗: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="263"/>
        <source>pa_ext_stream_restore_read() failed</source>
        <translation>pa_ext_stream_restore_read() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="281"/>
        <location filename="../pavucontrol.cc" line="556"/>
        <source>Failed to initialize device restore extension: %s</source>
        <translation>デバイスの復元拡張を初期化できません: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="302"/>
        <source>pa_ext_device_restore_read_sink_formats() failed</source>
        <translation>pa_ext_device_restore_read_sink_formats() 失敗しました</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="320"/>
        <location filename="../pavucontrol.cc" line="569"/>
        <source>Failed to initialize device manager extension: %s</source>
        <translation>デバイスマネージャーの拡張を初期化できません: %s</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="339"/>
        <source>pa_ext_device_manager_read() failed</source>
        <translation>pa_ext_device_manager_read() 失敗しました</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="356"/>
        <source>pa_context_get_sink_info_by_index() failed</source>
        <translation>pa_context_get_sink_info_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="369"/>
        <source>pa_context_get_source_info_by_index() failed</source>
        <translation>pa_context_get_source_info_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="382"/>
        <location filename="../pavucontrol.cc" line="395"/>
        <source>pa_context_get_sink_input_info() failed</source>
        <translation>pa_context_get_sink_input_info() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="408"/>
        <source>pa_context_get_client_info() failed</source>
        <translation>pa_context_get_client_info() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="418"/>
        <location filename="../pavucontrol.cc" line="483"/>
        <source>pa_context_get_server_info() failed</source>
        <translation>pa_context_get_server_info() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="431"/>
        <source>pa_context_get_card_info_by_index() failed</source>
        <translation>pa_context_get_card_info_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="474"/>
        <source>pa_context_subscribe() failed</source>
        <translation>pa_context_subscribe() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="490"/>
        <source>pa_context_client_info_list() failed</source>
        <translation>pa_context_client_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="497"/>
        <source>pa_context_get_card_info_list() failed</source>
        <translation>pa_context_get_card_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="504"/>
        <source>pa_context_get_sink_info_list() failed</source>
        <translation>pa_context_get_sink_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="511"/>
        <source>pa_context_get_source_info_list() failed</source>
        <translation>pa_context_get_source_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="518"/>
        <source>pa_context_get_sink_input_info_list() failed</source>
        <translation>pa_context_get_sink_input_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="525"/>
        <source>pa_context_get_source_output_info_list() failed</source>
        <translation>pa_context_get_source_output_info_list() 失敗</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="584"/>
        <location filename="../pavucontrol.cc" line="635"/>
        <source>Connection failed, attempting reconnect</source>
        <translation>接続に失敗しました。再接続しています</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="607"/>
        <location filename="../pavucontrol.cc" line="663"/>
        <source>PulseAudio Volume Control</source>
        <translation>PulseAudio 音量調節</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="622"/>
        <source>Connection to PulseAudio failed. Automatic retry in 5s.&lt;br&gt;&lt;br&gt;In this case this is likely because PULSE_SERVER in the Environment/X11 Root Window Propertiesor default-server in client.conf is misconfigured.&lt;br&gt;This situation can also arrise when PulseAudio crashed and left stale details in the X11 Root Window.&lt;br&gt;If this is the case, then PulseAudio should autospawn again, or if this is not configured you shouldrun start-pulseaudio-x11 manually.</source>
        <translation>PulseAudio への接続に失敗しました。5 秒後に自動的に再試行します。&lt;br&gt;&lt;br&gt;この場合、環境/X11 ルートウィンドウのプロパティの PULSE_SERVER または client.conf の default-server の設定が間違っている可能性があります。&lt;br&gt;この状況は PulseAudio がクラッシュして X11 ルートウィンドウに古い情報が残った場合にも発生する可能性があります。&lt;br&gt;この場合 PulseAudio は再度自動生成されます。これが設定されていない場合は、start-pulseaudio-X11 を手動で実行する必要があります。</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="671"/>
        <source>Select a specific tab on load.</source>
        <translation>起動してタブ&lt;tab&gt;を選択する。</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="674"/>
        <source>Retry forever if pa quits (every 5 seconds).</source>
        <translation>PulseAudio が終了した場合、(5 秒毎に)再試行を繰り返す。</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="677"/>
        <source>Maximize the window.</source>
        <translation>ウィンドウを最大化する。</translation>
    </message>
    <message>
        <location filename="../pavucontrol.cc" line="702"/>
        <source>Fatal Error: Unable to connect to PulseAudio</source>
        <translation>致命的なエラー: PulseAudio に接続できません</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../rolewidget.cc" line="59"/>
        <source>pa_ext_stream_restore_write() failed</source>
        <translation>pa_ext_stream_restore_write() 失敗</translation>
    </message>
</context>
<context>
    <name>SinkInputWidget</name>
    <message>
        <location filename="../sinkinputwidget.cc" line="36"/>
        <source>on</source>
        <translation>オン</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="39"/>
        <source>Terminate Playback</source>
        <translation>再生を終了</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="52"/>
        <source>Unknown output</source>
        <translation>不明な出力</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="63"/>
        <source>pa_context_set_sink_input_volume() failed</source>
        <translation>pa_context_set_sink_input_volume() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="78"/>
        <source>pa_context_set_sink_input_mute() failed</source>
        <translation>pa_context_set_sink_input_mute() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="88"/>
        <source>pa_context_kill_sink_input() failed</source>
        <translation>pa_context_kill_sink_input() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkinputwidget.cc" line="113"/>
        <source>pa_context_move_sink_input_by_index() failed</source>
        <translation>pa_context_move_sink_input_by_index() 失敗</translation>
    </message>
</context>
<context>
    <name>SinkWidget</name>
    <message>
        <location filename="../sinkwidget.cc" line="81"/>
        <source>pa_context_set_sink_volume_by_index() failed</source>
        <translation>pa_context_set_sink_volume_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="96"/>
        <source>pa_context_set_sink_mute_by_index() failed</source>
        <translation>pa_context_set_sink_mute_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="110"/>
        <source>pa_context_set_default_sink() failed</source>
        <translation>pa_context_set_default_sink() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="126"/>
        <source>pa_context_set_sink_port_by_index() failed</source>
        <translation>pa_context_set_sink_port_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../sinkwidget.cc" line="166"/>
        <source>pa_ext_device_restore_save_sink_formats() failed</source>
        <translation>pa_ext_device_restore_save_sink_formats() 失敗しました</translation>
    </message>
</context>
<context>
    <name>SourceOutputWidget</name>
    <message>
        <location filename="../sourceoutputwidget.cc" line="35"/>
        <source>from</source>
        <translation>から</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="39"/>
        <source>Terminate Recording</source>
        <translation>録音を終了</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="59"/>
        <source>Unknown input</source>
        <translation>不明な入力</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="71"/>
        <source>pa_context_set_source_output_volume() failed</source>
        <translation>pa_context_set_source_output_volume() 失敗しました</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="86"/>
        <source>pa_context_set_source_output_mute() failed</source>
        <translation>pa_context_set_source_output_mute() 失敗しました</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="97"/>
        <source>pa_context_kill_source_output() failed</source>
        <translation>pa_context_kill_source_output() 失敗</translation>
    </message>
    <message>
        <location filename="../sourceoutputwidget.cc" line="124"/>
        <source>pa_context_move_source_output_by_index() failed</source>
        <translation>pa_context_move_source_output_by_index() 失敗</translation>
    </message>
</context>
<context>
    <name>SourceWidget</name>
    <message>
        <location filename="../sourcewidget.cc" line="35"/>
        <source>pa_context_set_source_volume_by_index() failed</source>
        <translation>pa_context_set_source_volume_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="50"/>
        <source>pa_context_set_source_mute_by_index() failed</source>
        <translation>pa_context_set_source_mute_by_index() 失敗</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="64"/>
        <source>pa_context_set_default_source() failed</source>
        <translation>pa_context_set_default_source() 失敗</translation>
    </message>
    <message>
        <location filename="../sourcewidget.cc" line="80"/>
        <source>pa_context_set_source_port_by_index() failed</source>
        <translation>pa_context_set_source_port_by_index() 失敗</translation>
    </message>
</context>
<context>
    <name>StreamWidget</name>
    <message>
        <location filename="../streamwidget.ui" line="14"/>
        <source>Form</source>
        <translation>フォーム</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="32"/>
        <source>Device Title</source>
        <translation>デバイスのタイトル</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="52"/>
        <source>direction</source>
        <translation>方向</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="59"/>
        <source>device</source>
        <translation>デバイス</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="66"/>
        <source>Mute audio</source>
        <translation>音量をミュートします</translation>
    </message>
    <message>
        <location filename="../streamwidget.ui" line="79"/>
        <source>Lock channels together</source>
        <translation>各チャンネルを同時に</translation>
    </message>
    <message>
        <location filename="../streamwidget.cc" line="34"/>
        <source>Terminate</source>
        <translation>終了</translation>
    </message>
</context>
</TS>
