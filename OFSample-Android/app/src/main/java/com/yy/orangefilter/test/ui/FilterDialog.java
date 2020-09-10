package com.yy.orangefilter.test.ui;

import android.content.DialogInterface;
import android.graphics.drawable.Drawable;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import com.yy.orangefilter.test.R;
import com.yy.orangefilter.test.adapter.FilterAdapter;
import com.yy.orangefilter.test.base.BaseAdapter;
import com.yy.orangefilter.test.bean.FilterBean;
import com.yy.orangefilter.test.helper.OrangeHelper;

import java.util.List;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import me.shaohui.bottomdialog.BaseBottomDialog;

/**
 * 滤镜
 */
public class FilterDialog extends BaseBottomDialog {

    private RecyclerView recyclerView;
    private TextView tvEffectName;
    private TextView tvEffectVal;
    private SeekBar mSeekBar;
    private List<FilterBean> list;
    private OnItemSelectedListener listener;
    private int currentPosition = 0;

    public FilterDialog(List<FilterBean> list,
                        OnItemSelectedListener listener) {
        this.list = list;
        this.listener = listener;
    }

    @Override
    public int getLayoutRes() {
        return R.layout.layout_menu;
    }

    @Override
    public void bindView(View view) {
        recyclerView = view.findViewById(R.id.rvBeauty);
        tvEffectName = view.findViewById(R.id.tvEffectName);
        tvEffectVal = view.findViewById(R.id.tvEffectVal);
        mSeekBar = view.findViewById(R.id.seekEffect);

        recyclerView.setLayoutManager(new LinearLayoutManager(getContext(),
                LinearLayoutManager.HORIZONTAL, false));
        DividerItemDecoration decoration = new DividerItemDecoration(getContext(),
                DividerItemDecoration.HORIZONTAL);
        Drawable drawable = ContextCompat.getDrawable(getContext(), R.drawable.item_decoration);
        decoration.setDrawable(drawable);
        recyclerView.addItemDecoration(decoration);
        final FilterAdapter adapter = new FilterAdapter(list);
        adapter.setOnItemClickListener(new BaseAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                currentPosition = position;
                listener.onItemSelected(position);
                adapter.selectItem(position);
                if (position == 0) {
                    mSeekBar.setVisibility(View.INVISIBLE);
                    tvEffectName.setVisibility(View.INVISIBLE);
                    tvEffectVal.setVisibility(View.INVISIBLE);
                } else {
                    mSeekBar.setVisibility(View.VISIBLE);
                    tvEffectName.setVisibility(View.VISIBLE);
                    tvEffectVal.setVisibility(View.VISIBLE);
                    tvEffectName.setText(list.get(position).getOptionName());
                    OrangeHelper.EffectParam paramf = list.get(position).getOf_paramf();
                    if (paramf != null) {
                        int percent =
                                (int) (((paramf.curVal - paramf.minVal) * 1.0 /
                                        (paramf.maxVal - paramf.minVal)) *
                                        100);
                        mSeekBar.setProgress(percent);
                        tvEffectVal.setText(paramf.curVal + "");
                    }
                }
            }
        });
        recyclerView.setAdapter(adapter);

        mSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    OrangeHelper.EffectParam paramf = list.get(currentPosition).getOf_paramf();
                    int val = 0;
                    if (paramf != null) {
                        float distance = paramf.maxVal - paramf.minVal;
                        float percent = (float) (progress / 100.0);
                        float min = paramf.minVal;
                        val = (int) ((distance * percent + min));
                        tvEffectVal.setText(val + "");
                    }
                    list.get(currentPosition).getOf_paramf().curVal = val;
                    // Log.e("chen", "onProgressChanged: "+val );
                    listener.onSeekChanged(list.get(currentPosition).getParamType(), val);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        init();
    }

    private void init() {
        for (int i = 1; i < list.size(); i++) {
            boolean isEnable = list.get(i).isEnable();
            if (isEnable) {
                currentPosition = i;
                mSeekBar.setVisibility(View.VISIBLE);
                tvEffectName.setVisibility(View.VISIBLE);
                tvEffectVal.setVisibility(View.VISIBLE);
                tvEffectName.setText(list.get(i).getOptionName());
                OrangeHelper.EffectParam paramf = list.get(i).getOf_paramf();
                if (paramf != null) {
                    int percent =
                            (int) (((paramf.curVal - paramf.minVal) * 1.0 /
                                    (paramf.maxVal - paramf.minVal)) *
                                    100);
                    mSeekBar.setProgress(percent);
                    tvEffectVal.setText(paramf.curVal + "");
                }
                return;
            }
        }

        mSeekBar.setVisibility(View.INVISIBLE);
        tvEffectName.setVisibility(View.INVISIBLE);
        tvEffectVal.setVisibility(View.INVISIBLE);
    }

    @Override
    public void onDismiss(@NonNull DialogInterface dialog) {
        super.onDismiss(dialog);
        listener.onDialogDismiss();
    }

    @Override
    public float getDimAmount() {
        return 0;
    }

    public interface OnItemSelectedListener {

        void onItemSelected(int index);

        void onSeekChanged(OrangeHelper.EffectParamType effectParam, int value);

        void onDialogDismiss();
    }
}
