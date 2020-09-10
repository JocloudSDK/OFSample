package com.yy.orangefilter.test.base;

import android.graphics.drawable.Drawable;
import android.util.SparseArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;
import java.util.List;

import androidx.recyclerview.widget.RecyclerView;

public abstract class BaseAdapter<T> extends RecyclerView.Adapter<BaseAdapter.VH> {

    protected List<T> mDatas;
    protected OnItemClickListener onItemClickListener;

    public BaseAdapter(List<T> datas) {
        this.mDatas = datas;
    }

    public abstract int getLayoutId(int viewType);

    @NotNull
    @Override
    public VH onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(getLayoutId(viewType), parent, false);
        final VH vh = new VH(view);
        onCreateViewHolder(vh, viewType);
        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (onItemClickListener != null) {
                    onItemClickListener.onItemClick(view, vh.getAdapterPosition());
                }
            }
        });
        return vh;
    }

    @Override
    public void onBindViewHolder(@NotNull VH holder, int position) {
        onBindViewHolder(holder, mDatas.get(position), position);
    }

    @Override
    public int getItemCount() {
        return mDatas.size();
    }

    public abstract void onCreateViewHolder(@NotNull VH holder, int viewType);

    public abstract void onBindViewHolder(@NotNull VH holder, T data, int position);

    /**
     *
     * Add data item.
     * Don't use adapter.notifyDataSetChanged() to update data set.
     * Use notifyItemInserted(position) and notifyItemRemoved(position) instead.
     * Otherwise there is no animation effect.
     *
     * ZH:
     * 添加数据 更新数据集不是用adapter.notifyDataSetChanged()而是notifyItemInserted(position)与notifyItemRemoved(position)
     * 否则没有动画效果
     *
     */
    public void addItem(T data) {
        if (mDatas == null) {
            mDatas = new ArrayList<>();
        }
        mDatas.add(data);
        notifyItemInserted(mDatas.size() - 1);
    }

    /**
     *
     * Add data item.
     * Don't use adapter.notifyDataSetChanged() to update data set.
     * Use notifyItemInserted(position) and notifyItemRemoved(position) instead.
     * Otherwise there is no animation effect.
     *
     * ZH:
     * 添加数据 更新数据集不是用adapter.notifyDataSetChanged()而是notifyItemInserted(position)与notifyItemRemoved(position)
     * 否则没有动画效果
     *
     */
    public void addItem(int postion, T data) {
        if (mDatas == null) {
            mDatas = new ArrayList<>();
        }
        mDatas.add(postion, data);
        notifyItemInserted(postion);
    }

    public int indexOf(T data) {
        if (mDatas == null) {
            return -1;
        }
        return mDatas.indexOf(data);
    }

    /**
     *
     * updata item data
     *
     * ZH:
     * 更新数据
     *
     */
    public void updateItem(int postion, T data) {
        notifyItemChanged(postion, data);
    }

    public List<T> getData() {
        return mDatas;
    }

    /**
     *
     * The addAll method is important too.
     * Call notifyDataSetChanged() will be useless if use this.list = mList to set Data source.
     * notifyDataSetChanged() should be call when data source changes.
     * It will not make data source changes by use this.list = mList.
     *
     * ZH：
     * 这里用addAll也很重要，如果用this.list = mList，调用notifyDataSetChanged()无效，notifyDataSetChanged()数据源发生改变的时候调用的，this.list
     * = mList，list并没有发生改变
     *
     */
    public void setData(List<T> datas) {
        if (mDatas == null) {
            mDatas = new ArrayList<>();
        }

        mDatas.clear();
        mDatas.addAll(datas);
        notifyDataSetChanged();
    }

    /**
     *
     * Delete data item by index
     *
     * ZH:
     * 删除Item数据
     *
     */
    public void deleteItem(int posion) {
        if (mDatas == null || mDatas.isEmpty()) {
            return;
        }

        mDatas.remove(posion);
        notifyItemRemoved(posion);
    }

    /**
     *
     * Delete data item by data object.
     *
     * ZH:
     * 删除Items数据
     *
     */
    public void deleteItem(T data) {
        if (mDatas == null || mDatas.isEmpty()) {
            return;
        }

        int index = mDatas.indexOf(data);
        mDatas.remove(data);
        notifyItemRemoved(index);
    }

    public void clear() {
        if (mDatas == null || mDatas.isEmpty()) {
            return;
        }

        mDatas.clear();
        notifyDataSetChanged();
    }

    public interface OnItemClickListener {

        void onItemClick(View view, int position);
    }

    public void setOnItemClickListener(OnItemClickListener listener) {
        this.onItemClickListener = listener;
    }

    public static class VH extends RecyclerView.ViewHolder {

        private SparseArray<View> mViews;

        private VH(View v) {
            super(v);
            mViews = new SparseArray<>();
        }

        public <T extends View> T getView(int id) {
            View v = mViews.get(id);
            if (v == null) {
                v = itemView.findViewById(id);
                mViews.put(id, v);
            }
            return (T) v;
        }

        public void setText(int id, String value) {
            TextView view = getView(id);
            view.setText(value);
        }

        public void setImageDrawable(int id, Drawable drawable) {
            ImageView view = getView(id);
            view.setImageDrawable(drawable);
        }

        public void setImageRes(int id, int imgRes) {
            ImageView view = getView(id);
            view.setImageResource(imgRes);
        }
    }
}
