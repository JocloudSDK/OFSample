package com.yy.orangefilter.test.ui;

import android.content.DialogInterface;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

import com.yy.orangefilter.test.R;
import com.yy.orangefilter.test.adapter.GesturesAdapter;
import com.yy.orangefilter.test.base.BaseAdapter;
import com.yy.orangefilter.test.bean.GestureBean;
import com.yy.orangefilter.test.utils.MyDividerItemDecoration;

import java.sql.ResultSet;
import java.util.List;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.GridLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import me.shaohui.bottomdialog.BaseBottomDialog;

/**
 * 手势贴纸
 */
public class GesturesDialog extends BaseBottomDialog {

    private RecyclerView recyclerView;
    private TextView tvEffectName;
    private TextView tvEffectVal;
    private SeekBar mSeekBar;
    private List<GestureBean> list;
    private OnItemSelectedListener listener;

    public GesturesDialog(List<GestureBean> list, OnItemSelectedListener listener) {
        this.list = list;
        this.listener = listener;
    }

    @Override
    public int getLayoutRes() {
        return R.layout.layout_menu;
    }

    @Override
    public void bindView(View v) {
        recyclerView = v.findViewById(R.id.rvBeauty);
        tvEffectName = v.findViewById(R.id.tvEffectName);
        tvEffectVal = v.findViewById(R.id.tvEffectVal);
        mSeekBar = v.findViewById(R.id.seekEffect);

        GridLayoutManager layoutManager = new GridLayoutManager(getContext(),5);
        MyDividerItemDecoration decoration = new MyDividerItemDecoration(getContext(),
                MyDividerItemDecoration.BOTH_SET,R.drawable.item_decoration2);
        recyclerView.addItemDecoration(decoration);
        recyclerView.setLayoutManager(layoutManager);
        final GesturesAdapter adapter = new GesturesAdapter(list);
        adapter.setOnItemClickListener(new BaseAdapter.OnItemClickListener() {
            @Override
            public void onItemClick(View view, int position) {
                if (position == 0) {
                    listener.onCleanSticker();
                    adapter.resetItem();
                } else {
                    boolean result = adapter.selectItem(position);
                    listener.onItemSelected(position,result);
                }
            }
        });
        recyclerView.setAdapter(adapter);

        init();
    }

    @Override
    public void onDismiss(@NonNull DialogInterface dialog) {
        super.onDismiss(dialog);
        listener.onDialogDismiss();
    }

    private void init() {
        mSeekBar.setVisibility(View.GONE);
        tvEffectName.setVisibility(View.GONE);
        tvEffectVal.setVisibility(View.GONE);
    }

    @Override
    public float getDimAmount() {
        return 0;
    }

    public interface OnItemSelectedListener {
        void onCleanSticker();
        void onItemSelected(int index, boolean result);
        void onDialogDismiss();
    }
}
