#!/bin/sh

ctx_list=" \
	CardWidget \
	ChannelWidget \
	DeviceWidget \
	MainWindow \
	QObject \
	StreamWidget"

po_dir="../../po"
for lang in `cat $po_dir/LINGUAS`; do
	po_file="$po_dir/$lang".po
	for ctx in $ctx_list; do
		echo $ctx
		po2ts -i "$po_file" -o tmp_"$ctx".ts -c $ctx -t pavucontrol-qt.ts
	done
	echo $po_file
	# merge the files
	output_file=pavucontrol-qt_"$lang".ts
	lconvert tmp_*.ts -o $output_file
	lupdate -disable-heuristic similartext -disable-heuristic number -target-language "$lang" -no-obsolete ../*.cc ../*.h ../*.ui -ts $output_file
	rm tmp_*.ts
done

